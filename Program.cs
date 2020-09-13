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
using Tea;
using Utils;

namespace ModelCoder
{
	class Program
	{
		private static readonly int encodeSize = 3000;

		private static bool encodeFile(string path, uint[] key)
		{
			Console.WriteLine("[OUTPUT] Reading: " + path);
			byte[] file_bytes;
			try {
				file_bytes = File.ReadAllBytes(path);
			} catch {
				Console.WriteLine("[OUTPUT] File '" + path + "' not found");
				return false;
			}

			ITeaCoding tea = new TeaCoding();
			var block = new uint[2];
			for (uint i = 0; i < encodeSize; i += 2) {
				Console.WriteLine(i + " start iteration");
				block[0] = file_bytes[i];
				block[1] = file_bytes[i + 1];
				Console.WriteLine(i + " start encoding");
				tea.code(block, key);
			}

			return true;
		}
		
		[STAThread]
		static void Main(string[] args)
		{
			IUtil util = new Util();

			string fpath;
			if (args.Length > 0) {
				fpath = args[0];
			} else {
		        Console.WriteLine("[OUTPUT] You can open file(s) with this program");
		        Console.WriteLine("[OUTPUT] Or drag'n'drop on it");
		        Console.Write("[INPUT] Enter filename (without spaces): ");
		        fpath = Console.ReadLine();
			}

			Console.Write("[INPUT] Enter key: ");
			uint[] uKey = util.ConvertKey(Console.ReadLine());
			
			Console.WriteLine("[OUTPUT] Reading file(s)...");
			if (args.Length > 1) {
				foreach (string i in args)
					encodeFile(i, uKey);
			} else {
				encodeFile(fpath, uKey);
			}

			Console.Write("Press any key to continue . . . ");
			Console.ReadKey(true);
		}
	}
}
