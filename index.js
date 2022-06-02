const chalk = require('chalk');
const yargs = require('yargs/yargs')
const { hideBin } = require('yargs/helpers')
const fs = require('fs');
const fse = require('fs-extra')
const path = require('path');
const ora = require('ora');
const glob = require("glob");
const tea = require('mta-tea');
const crypto = require('crypto');

const error = chalk.bold.red;

const argv = yargs(hideBin(process.argv))
.usage('Usage: $0 -pass [pass] -res [path]')
.example('$0 -pass encryptme -res resourceName', 'Encrypt resourceName')
.options('pass', {
    alias: 'p',
    describe: 'Password for the encryption',
    type: 'string',
    coerce: arg =>
    arg && arg.length >= 6 ? arg : undefined
})
.options('res', {
    alias: 'r',
    describe: 'Resource folder path',
    type: 'string',
    coerce: arg =>
    arg && fs.existsSync(path.resolve(__dirname, arg)) && fs.lstatSync(path.resolve(__dirname, arg)).isDirectory() ? arg : undefined
})
.demandOption(['pass'], error("Include password with 6 symbols or more!"))
.demandOption(['res'], error("Incorrect or no resource folder selected!"))
.argv;

const encodeSize = 3000; // byte count to encode

let getDirectories = (src, callback) => {
    glob(src + '/**/*', callback);
}

const extensions = ['.col', '.txd', '.dff']

let spinner = ora('Getting files list').start();
getDirectories(argv.res, (err, res) => {
    if (err) {
        spinner.fail('Something went wrong!')
    } else {
        let files = res.filter(element => fs.lstatSync(path.resolve(__dirname, element)).isFile() && extensions.includes(path.extname(element)))
        spinner.succeed()

        spinner = ora('Encoding files').start();
        files.forEach(file => {
            const newFilePath = file + 'c';

            // Encoding first $encodeSize bytes from file
            const key = crypto.createHash('sha256').update(argv.pass).digest('hex').toUpperCase();
            const data = fs.readFileSync(file);
            let array = [];
            for (let i = 0; i < encodeSize; i++)
                array.push(tea.encode(data[i].toString(), key));

            // Checking file for existing, if exists - deleting it
            try {
                fs.accessSync(newFilePath);
                fs.rmSync(newFilePath);
            } catch (err) {
                // console.log("access err: " + err);
            }
            
            let fd = fs.openSync(newFilePath, 'a+'); // Open file
            fs.writeSync(fd, array.join(""), 0); // Writing encoded part in start of file
            fs.writeSync(fd, data, encodeSize, data.length-encodeSize); // Writing original part after encoded part
            fs.closeSync(fd); // Close file

            // fse.removeSync(file); // Remove original non-encoded file
        });
        spinner.succeed()
    }
})
