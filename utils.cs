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
		string Base64Encode(string plainText);
		//string Base64Encode(byte[] plainText);
		string Base64Decode(string base64EncodedData);
	}
	public partial class Util: IUtil
	{
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
		#region Base64 encoding/decoding
		public string Base64Encode(string plainText)
		{
			var plainTextBytes = Encoding.UTF8.GetBytes(plainText);
			return Convert.ToBase64String(plainTextBytes);
		}
		/*public string Base64Encode(byte[] data)
			return Convert.ToBase64String(data);
		public char[] Base64Encode(byte[] data)
		{
			int length, length2;
			int blockCount;
			int paddingCount;
		
			length = data.Length;
		
			if ((length % 3) == 0)
			{
				paddingCount = 0;
				blockCount = length / 3;
			}
			else
			{
				paddingCount = 3 - (length % 3);
				blockCount = (length + paddingCount) / 3;
			}
		
			length2 = length + paddingCount;
			
			Console.WriteLine("length: " + length);
			Console.WriteLine("length2: " + length2);
			Console.WriteLine("blockCount: " + blockCount);
			Console.WriteLine("paddingCount: " + paddingCount);
		
			byte[] source2;
			source2 = new byte[length2];
		
			for (int x = 0; x < length2; x++)
				source2[x] = x < length ? data[x] : (byte)0;
		
			byte b1, b2, b3;
			byte temp, temp1, temp2, temp3, temp4;
			var buffer = new byte[blockCount * 4];
			var result = new char[blockCount * 4];
		
			for (int x = 0; x < blockCount; x++)
			{
				b1 = source2[x * 3];
				b2 = source2[x * 3 + 1];
				b3 = source2[x * 3 + 2];
		
				temp1 = (byte)((b1 & 252) >> 2);
		
				temp = (byte)((b1 & 3) << 4);
				temp2 = (byte)((b2 & 240) >> 4);
				temp2 += temp;
		
				temp = (byte)((b2 & 15) << 2);
				temp3 = (byte)((b3 & 192) >> 6);
				temp3 += temp;
		
				temp4 = (byte)(b3 & 63);
		
				buffer[x * 4] = temp1;
				buffer[x * 4 + 1] = temp2;
				buffer[x * 4 + 2] = temp3;
				buffer[x * 4 + 3] = temp4;
			}
		
			for (int x = 0; x < blockCount * 4; x++)
				result[x] = SixBitToChar(buffer[x]);
		
			switch (paddingCount)
			{
				case 0:
					break;
				case 1:
					result[blockCount * 4 - 1] = '=';
					break;
				case 2:
					result[blockCount * 4 - 1] = '=';
					result[blockCount * 4 - 2] = '=';
					break;
				default:
					break;
			}
		
			return result;
		}
		private static char SixBitToChar(byte b)
		{
			var lookupTable = new char[64] {
				'A','B','C','D','E','F','G','H','I','J','K','L','M',
				'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
				'a','b','c','d','e','f','g','h','i','j','k','l','m',
				'n','o','p','q','r','s','t','u','v','w','x','y','z',
				'0','1','2','3','4','5','6','7','8','9','+','/'
			};
		
			if ((b >= 0) && (b <= 63))
			{
				return lookupTable[(int)b];
			}
			else
			{
				return ' ';
			}
		}*/

		public string Base64Decode(string base64EncodedData)
		{
			var base64EncodedBytes = Convert.FromBase64String(base64EncodedData);
			return Encoding.UTF8.GetString(base64EncodedBytes);
		}
		#endregion
	}
}
