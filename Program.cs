/*
 * Created by SharpDevelop.
 * User: JDM17
 * Date: 23.09.2020
 * Time: 17:48
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;
using System.IO;
using System.Text;
using Tea;
using Utils;

namespace ModelCoder
{
	class Program
	{
		private static IUtil util;
		private static ITeaCoding tea;
		private static readonly uint blockSize = 12;
		private static readonly uint blockNums = 3000;
		private static readonly uint encodeSize = blockSize * blockNums;
		const int byteCount = 4;

		private static string checkBlockBytes(byte block)
		{
			string newBlock = block.ToString();
			int size = Encoding.UTF8.GetByteCount(newBlock);
			//Console.WriteLine("newBlock: " + newBlock);
			//Console.WriteLine("newBlock size: " + size);
			if (size < byteCount)
				for (int i = 0; i < (byteCount - size); i++)
					newBlock += '\0';
			//Console.WriteLine("newblock size: " + System.Text.ASCIIEncoding.UTF8.GetByteCount(newBlock));
			return newBlock;
		}
		
		private static bool encodeFile(string path, uint[] key)
		{
			#region Reading file
			Console.WriteLine("[OUTPUT] Reading: " + path);
			byte[] file_bytes;
			try {
				file_bytes = File.ReadAllBytes(path);
			} catch {
				Console.WriteLine("[OUTPUT] File '" + path + "' not found");
				return false;
			}
			#endregion
			
			/*
			string[] test = {"22\0\0\0\0\0\0", "1\0\0\0\0\0\0\0"};
			foreach (var i in test)
			{
				Console.WriteLine(i + " bytes: " + Encoding.UTF8.GetByteCount(i));
				Console.WriteLine(i + " b64: " + util.Base64Encode(i));
			}
			string blockCheck = checkBlockBytes(file_bytes[0]);
			Console.WriteLine("first file block: " + blockCheck);
			Console.WriteLine("first file block size: " + Encoding.UTF8.GetByteCount(blockCheck));
			blockCheck = util.Base64Encode(blockCheck);
			Console.WriteLine("first file block b64e: " + blockCheck);
			Console.WriteLine("first file block b64d: " + util.Base64Decode(blockCheck));
			*/

			// TEA and Base64 encoding
			var block1 = new uint[2];
			string result = string.Empty;
			for (uint i = 0; i < 8; i += 2)
			{
				//block1[0] = file_bytes[i];
				//block1[1] = file_bytes[i + 1];
				
				block1[0] = util.ConvertStringToUInt(checkBlockBytes(file_bytes[i]));
				block1[1] = util.ConvertStringToUInt(checkBlockBytes(file_bytes[i + 1]));
				
				//block1[0] = Convert.ToUInt32(checkBlockBytes(file_bytes[i]));
				//block1[1] = Convert.ToUInt32(checkBlockBytes(file_bytes[i + 1]));
				
				Console.WriteLine();
				
				foreach (var b1 in block1)
					Console.WriteLine("st block1: " + b1);
				
				tea.encode(block1, key);
				
				foreach (var b1 in block1)
					Console.WriteLine("enc block1: " + b1);
				
				Console.WriteLine("block1[0] bytes:");
				foreach (var j in BitConverter.GetBytes(block1[0]))
					Console.WriteLine("  " + j);
				
				Console.WriteLine("block1[1] bytes:");
				foreach (var j in BitConverter.GetBytes(block1[1]))
					Console.WriteLine("  " + j);

				//Console.WriteLine(util.Base64Encode(block1[0].ToString()));
				//Console.WriteLine(util.Base64Encode(block1[1].ToString()));
				//Console.WriteLine("b64: " + util.Base64Encode(block1[0].ToString() + block1[1].ToString()));
				
				Console.WriteLine();
				
				//result += util.Base64Encode(BitConverter.GetBytes(block[0])) +
				//	util.Base64Encode(BitConverter.GetBytes(block[1]));
				result += util.Base64Encode(block1[0].ToString()) +
					util.Base64Encode(block1[1].ToString());
			}
			Console.WriteLine("Result: " + result);
			Console.WriteLine("[OUTPUT] File '" + path + "' has been TEA and Base64 encoded");

			#region Writing file with encoded and non-encoded part
			string encodedPath = path + ".enc";
			// encoded part
			File.WriteAllText(encodedPath, result);
			// non-encoded part
			FileStream ofile = File.OpenWrite(encodedPath);
			ofile.Seek(encodeSize, 0);
			for (uint i = blockNums; i < file_bytes.Length; i++)
				ofile.WriteByte(file_bytes[i]);
			ofile.Close();
			Console.WriteLine("[OUTPUT] File '" + encodedPath + "' successfully written");
			#endregion

			return true;
		}
		
		[STAThread]
		static void Main(string[] args)
		{
			util = new Util();
			tea = new TeaCoding();
			
			Console.WriteLine(util.Base64Decode("osz/GcOVC5s="));
			Console.WriteLine(util.Base64Decode("XsXA4M0B2X0="));

			string fpath;
			if (args.Length > 0)
				fpath = args[0];
			else
			{
				Console.WriteLine("[OUTPUT] You can open file(s) with this program");
				Console.WriteLine("[OUTPUT] Or drag'n'drop on it");
				Console.Write("[INPUT] Enter filename (without spaces): ");
				fpath = Console.ReadLine();
			}

			Console.Write("[INPUT] Enter key: ");
			uint[] uKey = util.ConvertKey(Console.ReadLine());

			Console.WriteLine("[OUTPUT] Reading file(s)...");
			if (args.Length > 0)
				foreach (string i in args)
					encodeFile(i, uKey);
			else
				encodeFile(fpath, uKey);

			Console.Write("Press any key to continue . . . ");
			Console.ReadKey(true);
		}
	}
}
