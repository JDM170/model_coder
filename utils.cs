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
		uint[] ConvertKey(string input);
		uint ConvertStringToUInt(string input);
		string ConvertUIntToString(uint input);
	}
	public partial class Util: IUtil
	{
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
	}
}
