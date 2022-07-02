const chalk = require('chalk');
const yargs = require('yargs/yargs')
const { hideBin } = require('yargs/helpers')
const fs = require('fs');
const path = require('path');
const ora = require('ora');
const glob = require("glob");
const tea = require('mta-tea');
const crypto = require('crypto');

const error = chalk.bold.red;

const argv = yargs(hideBin(process.argv))
.usage('Usage: $0 -pass [pass] -res [path]')
.example('$0 -pass decryptme -res resourceName', 'Decrypt resourceName')
.options('pass', {
    alias: 'p',
    describe: 'Password for the decryption',
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

const blockSize = 12; // encoded block length
const encodeSize = 3000; // encoded byte count
const decodeSize = blockSize * encodeSize;

let getDirectories = (src, callback) => {
    glob(src + '/**/*', callback);
}

const extensions = ['.colrw', '.colc', '.txdrw', '.txdc', '.dffrw', '.dffc']

let spinner = ora('Getting files list').start();
getDirectories(argv.res, (err, res) => {
    if (err) {
        spinner.fail('Something went wrong!')
    } else {
        let files = res.filter(element => fs.lstatSync(path.resolve(__dirname, element)).isFile() && extensions.includes(path.extname(element)))
        spinner.succeed()

        spinner = ora('Decoding files').start();
        files.forEach(file => {
            const newFilePath = file + '.dec';

            // Decoding first $decodeSize bytes from file
            const key = crypto.createHash('md5').update(argv.pass).digest('hex').substring(0, 16);
            const data = fs.readFileSync(file);
            const encodedPart = data.toString().substring(0, decodeSize);
            let decoded = [];
            for (let i = 0; i < decodeSize; i += blockSize)
                decoded.push(tea.decode(encodedPart.substring(i, i + blockSize), key));

            // Checking the file for existence, if the file exists - deleting it
            try {
                fs.accessSync(newFilePath);
                fs.rmSync(newFilePath);
            } catch (err) {
                // console.log("access err: " + err);
            }

            let fd = fs.openSync(newFilePath, 'a+'); // Open file
            fs.writeSync(fd, Buffer.from(decoded), 0); // Writing decoded part in start of file
            fs.writeSync(fd, data, decodeSize, data.length - decodeSize); // Writing original part after decoded part
            fs.closeSync(fd); // Close file
        });
        spinner.succeed()
    }
})
