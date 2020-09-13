/*
 * Created by SharpDevelop.
 * User: JDM17
 * Date: 23.09.2020
 * Time: 17:50
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;

namespace Tea
{
	public interface ITeaCoding
	{
		void code(uint[] v, uint[] k);
		//void decode(uint[] v, uint[] k);
	}
	public partial class TeaCoding: ITeaCoding
	{
		private readonly uint c_delta = 0x9E3779B9;

		/*
		public void encode(uint[] v, uint[] k) {
			uint v0 = v[0], v1 = v[1];
			uint sum = 0;
			//for(int i = 0; i < 32; i++) {
			for(uint i = 32; i > 0; i--) {
				//v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
				//sum += c_delta;
				//v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum >> 11) & 3]);

				//sum += c_delta;
				//v0 += (v1 << 4) + k[0] ^ v1 + sum ^ (v1 >> 5) + k[1];
				//v1 += (v0 << 4) + k[2] ^ v0 + sum ^ (v0 >> 5) + k[3];

				v0 += (v1 << 4 ^ v1 >> 5) + v1 ^ sum + k[sum & 3];
		        sum += c_delta;
		        v1 += (v0 << 4 ^ v0 >> 5) + v0 ^ sum + k[sum >> 11 & 3];
			}
			v[0] = v0;
			v[1] = v1;
		}
		*/
		public void code(uint[] v, uint[] k)
		{
		    uint y = v[0];
		    uint z = v[1];
		    uint sum = 0;
		    uint n = 32;

		    while (n-- > 0)
		    {
		        y += (z << 4 ^ z >> 5) + z ^ sum + k[sum & 3];
		        sum += c_delta;
		        z += (y << 4 ^ y >> 5) + y ^ sum + k[sum >> 11 & 3];
		    }

		    v[0] = y;
		    v[1] = z;
		}

		/*
		private readonly uint c_sum = 0xC6EF3720;
		public void decode(uint[] v, uint[] k) {
			uint v0 = v[0], v1 = v[1];
			uint sum = c_sum;
			for (int i = 0; i < 32; i++) {
				v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum >> 11) & 3]);
				sum -= c_delta;
				v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
			}
			v[0] = v0;
			v[1] = v1;
		}
		*/
	}
}
