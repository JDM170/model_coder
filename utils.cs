/*
 * Created by SharpDevelop.
 * User: JDM17
 * Date: 23.09.2020
 * Time: 20:59
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */

using System;
using System.Text;
using System.Security.Cryptography;

namespace Utils
{
	public interface IUtil
	{
		uint ConvertStringToUInt(string input);
		string ConvertUIntToString(uint input);
		uint[] ConvertKey(string input);
		string Base64Encode(string plainText);
		//string Base64Encode(byte[] plainText);
		string Base64Decode(string base64EncodedData);
	}
	public partial class Util: IUtil
	{
		#region Convert String to UInt and reverse
		public uint ConvertStringToUInt(string input)
		{
			uint output;
			output =  ((uint)input[0]);
			output += ((uint)input[1] << 8);
			output += ((uint)input[2] << 16);
			output += ((uint)input[3] << 24);
			return output;
		}
		public string ConvertUIntToString(uint input)
		{
			var output = new StringBuilder();
			output.Append((char)((input & 0xFF)));
			output.Append((char)((input >> 8) & 0xFF));
			output.Append((char)((input >> 16) & 0xFF));
			output.Append((char)((input >> 24) & 0xFF));
			return output.ToString();
		}
		#endregion
		#region Convert Key to MD5, then to UInt
		public uint[] ConvertKey(string input)
		{
			// MD5 Hashing	
			var md5 = MD5.Create();
			byte[] inputBytes = Encoding.ASCII.GetBytes(input);
			byte[] hashBytes = md5.ComputeHash(inputBytes);
			var sb = new StringBuilder();
			for (int i = 0; i < hashBytes.Length; i++)
				sb.Append(hashBytes[i].ToString("X2"));
			// Converting to uint[]
			string key = sb.ToString().ToLower().Substring(0, 16);
			var formattedKey = new uint[4];
			int j = 0;
			for (int i = 0; i < key.Length; i += 4)
				formattedKey[j++] = ConvertStringToUInt(key.Substring(i, 4));
			return formattedKey;
		}
		#endregion
		#region Base64 encoding/decoding
		public string Base64Encode(string plainText)
		{
			var plainTextBytes = Encoding.UTF8.GetBytes(plainText);
			return Convert.ToBase64String(plainTextBytes);
		}
		/*public string Base64Encode(byte[] data)
			return Convert.ToBase64String(data);
		*/
		public string Base64Decode(string base64EncodedData)
		{
			var base64EncodedBytes = Convert.FromBase64String(base64EncodedData);
//			foreach (var i in base64EncodedBytes)
//				Console.WriteLine(" " + i);
			return Encoding.UTF8.GetString(base64EncodedBytes);
		}
		#endregion
	}
}
