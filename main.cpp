#include "stdafx.h"
#include <stdio.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <ctime>

using namespace std;

extern "C" __global__ static void kernel(unsigned char* output, unsigned char* query, int queryLength, int N);
__device__ unsigned char* SHA256Compute(unsigned char* output, unsigned char* query, int queryLength, int idx, int idy, int idz);
__device__ bool SHATransform(unsigned int* expandedBuffer, int expandedBufferLen0, unsigned int* state, unsigned char* block, int blockLen0, unsigned int* _K, int _KLen0);
__device__ unsigned int RotateRight(unsigned int x, int n);
__device__ unsigned int Ch(unsigned int x, unsigned int y, unsigned int z);
__device__ unsigned int Maj(unsigned int x, unsigned int y, unsigned int z);
__device__ unsigned int sigma_0(unsigned int x);
__device__ unsigned int sigma_1(unsigned int x);
__device__ unsigned int Sigma_0(unsigned int x);
__device__ unsigned int Sigma_1(unsigned int x);
__device__ void DWORDToBigEndian(unsigned char* block, unsigned int* x, int digits);
__device__ void DWORDFromBigEndian(unsigned int* x, int xLen0, int digits, unsigned char* block, int blockLen0);
__device__ void CopyArray(unsigned char* SourceArray, int SourceArrayLen0, int SourceIndex, unsigned char* DestinationArray, int DestinationArrayLen0, int DestinationIndex, int Length);
extern "C" __global__ static void kernel(unsigned char* output, unsigned char* query, int queryLength, int N)
{
	int idx = blockIdx.x * blockDim.x + threadIdx.x;

	if (idx < N)
	{
		/*for(int v1 = 0x73; v1 < 0x74; v1++)
		{*/
		SHA256Compute(output, query, queryLength, idx & 0x000000FF, idx >> 8, 0x73);
		//}
	}
}
//__device__
__device__ unsigned char* SHA256Compute(unsigned char* output, unsigned char* query, int queryLength, int idx, int idy, int idz)
{

	long long Count = 0L;
	//password = 70 61 73 73 77 6f 72 64
	int MessageLength = 8;
	unsigned char * Message = new unsigned char[MessageLength];
	Message[0] = idx;
	Message[1] = idy;
	Message[2] = idz;
	Message[3] = 0x73;
	Message[4] = 0x77;
	Message[5] = 0x6f;
	Message[6] = 0x72;
	Message[7] = 0x64;
	unsigned int * _stateSHA256 = new unsigned int[8];
	_stateSHA256[(0)] = 1779033703u;
	_stateSHA256[(1)] = 3144134277u;
	_stateSHA256[(2)] = 1013904242u;
	_stateSHA256[(3)] = 2773480762u;
	_stateSHA256[(4)] = 1359893119u;
	_stateSHA256[(5)] = 2600822924u;
	_stateSHA256[(6)] = 528734635u;
	_stateSHA256[(7)] = 1541459225u;

	int _KLen0 = 64;
	unsigned int * _K = new unsigned int[64];
	_K[(0)] = 1116352408u;
	_K[(1)] = 1899447441u;
	_K[(2)] = 3049323471u;
	_K[(3)] = 3921009573u;
	_K[(4)] = 961987163u;
	_K[(5)] = 1508970993u;
	_K[(6)] = 2453635748u;
	_K[(7)] = 2870763221u;
	_K[(8)] = 3624381080u;
	_K[(9)] = 310598401u;
	_K[(10)] = 607225278u;
	_K[(11)] = 1426881987u;
	_K[(12)] = 1925078388u;
	_K[(13)] = 2162078206u;
	_K[(14)] = 2614888103u;
	_K[(15)] = 3248222580u;
	_K[(16)] = 3835390401u;
	_K[(17)] = 4022224774u;
	_K[(18)] = 264347078u;
	_K[(19)] = 604807628u;
	_K[(20)] = 770255983u;
	_K[(21)] = 1249150122u;
	_K[(22)] = 1555081692u;
	_K[(23)] = 1996064986u;
	_K[(24)] = 2554220882u;
	_K[(25)] = 2821834349u;
	_K[(26)] = 2952996808u;
	_K[(27)] = 3210313671u;
	_K[(28)] = 3336571891u;
	_K[(29)] = 3584528711u;
	_K[(30)] = 113926993u;
	_K[(31)] = 338241895u;
	_K[(32)] = 666307205u;
	_K[(33)] = 773529912u;
	_K[(34)] = 1294757372u;
	_K[(35)] = 1396182291u;
	_K[(36)] = 1695183700u;
	_K[(37)] = 1986661051u;
	_K[(38)] = 2177026350u;
	_K[(39)] = 2456956037u;
	_K[(40)] = 2730485921u;
	_K[(41)] = 2820302411u;
	_K[(42)] = 3259730800u;
	_K[(43)] = 3345764771u;
	_K[(44)] = 3516065817u;
	_K[(45)] = 3600352804u;
	_K[(46)] = 4094571909u;
	_K[(47)] = 275423344u;
	_K[(48)] = 430227734u;
	_K[(49)] = 506948616u;
	_K[(50)] = 659060556u;
	_K[(51)] = 883997877u;
	_K[(52)] = 958139571u;
	_K[(53)] = 1322822218u;
	_K[(54)] = 1537002063u;
	_K[(55)] = 1747873779u;
	_K[(56)] = 1955562222u;
	_K[(57)] = 2024104815u;
	_K[(58)] = 2227730452u;
	_K[(59)] = 2361852424u;
	_K[(60)] = 2428436474u;
	_K[(61)] = 2756734187u;
	_K[(62)] = 3204031479u;
	_K[(63)] = 3329325298u;

	int _bufferLen0 = 64;
	unsigned char* _buffer = new unsigned char[64];
	for (int i = 0; i < 64; i++) _buffer[i] = 0;

	int _WLen0 = 64;
	unsigned int * _W = new unsigned int[64];
	for (int i = 0; i < 64; i++) _W[i] = 0;

	int partInLen = MessageLength;
	int partInBase = 0;
	int bufferLen = (int)(Count & 0x3f);

	Count += (long long)partInLen;

	if (bufferLen > 0 && bufferLen + partInLen >= 64)
	{
		CopyArray(Message, MessageLength, partInBase, _buffer, _bufferLen0, bufferLen, 64 - bufferLen);
		partInBase += 64 - bufferLen;
		partInLen -= 64 - bufferLen;
		SHATransform(_W, _WLen0, _stateSHA256, _buffer, _bufferLen0, _K, _KLen0);
		bufferLen = 0;
	}
	while (partInLen >= 64)
	{
		CopyArray(Message, MessageLength, partInBase, _buffer, _bufferLen0, 0, 64);
		partInBase += 64;
		partInLen -= 64;
		SHATransform(_W, _WLen0, _stateSHA256, _buffer, _bufferLen0, _K, _KLen0);
	}
	if (partInLen > 0)
	{
		CopyArray(Message, MessageLength, partInBase, _buffer, _bufferLen0, bufferLen, partInLen);
	}

	int padLen = 64 - (int)(Count & 0x3f);
	if (padLen <= 8)
	{
		padLen += 64;
	}

	unsigned char* pad = new unsigned char[padLen];
	pad[(0)] = 0x80;
	for (int i = 1; i < padLen; i++)
	{
		pad[i] = (unsigned char)0x00;
	}

	long long bitCount = Count * 8L;

	pad[padLen - 8] = (unsigned char)((int)bitCount >> 56 & 0xff);
	pad[padLen - 7] = (unsigned char)((int)bitCount >> 48 & 0xff);
	pad[padLen - 6] = (unsigned char)((int)bitCount >> 40 & 0xff);
	pad[padLen - 5] = (unsigned char)((int)bitCount >> 32 & 0xff);
	pad[padLen - 4] = (unsigned char)((int)bitCount >> 24 & 0xff);
	pad[padLen - 3] = (unsigned char)((int)bitCount >> 16 & 0xff);
	pad[padLen - 2] = (unsigned char)((int)bitCount >> 8 & 0xff);
	pad[padLen - 1] = (unsigned char)(bitCount & 0xff);

	bufferLen = 0;
	partInLen = padLen;
	partInBase = 0;
	bufferLen = (int)(Count & 0x3f);
	Count += (long long)partInLen;

	if (bufferLen > 0 && bufferLen + partInLen >= 64)
	{
		CopyArray(pad, padLen, partInBase, _buffer, _bufferLen0, bufferLen, 64 - bufferLen);
		partInBase += 64 - bufferLen;
		partInLen -= 64 - bufferLen;
		SHATransform(_W, _WLen0, _stateSHA256, _buffer, _bufferLen0, _K, _KLen0);
		bufferLen = 0;
	}
	while (partInLen >= 64)
	{
		CopyArray(pad, padLen, partInBase, _buffer, _bufferLen0, 0, 64);
		partInBase += 64;
		partInLen -= 64;
		SHATransform(_W, _WLen0, _stateSHA256, _buffer, _bufferLen0, _K, _KLen0);
	}
	if (partInLen > 0)
	{
		CopyArray(pad, padLen, partInBase, _buffer, _bufferLen0, bufferLen, partInLen);
	}

	unsigned char * ResultHash = new unsigned char[32];
	DWORDToBigEndian(ResultHash, _stateSHA256, 8);

	bool cont = true;
	int t = 0;
	while (cont)
	{
		if (ResultHash[t] == query[t])
		{
			cont = true;
			if (t == 31)
			{
				//full match found
				printf("%d : *** match found ***n", idx);
				for (int i = 0; i< 32; i++)
				{
					printf("%02X", ResultHash[i]);
					if (i < MessageLength)
					{
						output[i] = Message[i];
					}
				}
				printf("n");
				break;
			}
		}
		else
		{
			cont = false;
			break;
		}
		t++;
	}

	free(ResultHash);
	free(query);
	free(Message);
	free(_K);
	free(_W);
	free(_stateSHA256);
	free(_buffer);

	return output;
}

__device__ bool SHATransform(unsigned int* x, int xLen, unsigned int* state, unsigned char* block, int blockLen0, unsigned int* _K, int _KLen0)
{
	unsigned int num = state[(0)];
	unsigned int num2 = state[(1)];
	unsigned int num3 = state[(2)];
	unsigned int num4 = state[(3)];
	unsigned int num5 = state[(4)];
	unsigned int num6 = state[(5)];
	unsigned int num7 = state[(6)];
	unsigned int num8 = state[(7)];
	DWORDFromBigEndian(x, xLen, 16, block, blockLen0);
	for (int i = 16; i < 64; i++)
	{
		x[(i)] = sigma_1(x[(i - 2)]) + x[(i - 7)] + sigma_0(x[(i - 15)]) + x[(i - 16)];
	}
	for (int i = 0; i < 64; i++)
	{
		unsigned int num9 = num8 + Sigma_1(num5) + Ch(num5, num6, num7) + _K[(i)] + x[(i)];
		unsigned int num10 = num4 + num9;
		unsigned int num11 = num9 + Sigma_0(num) + Maj(num, num2, num3);
		i++;
		num9 = num7 + Sigma_1(num10) + Ch(num10, num5, num6) + _K[(i)] + x[(i)];
		unsigned int num12 = num3 + num9;
		unsigned int num13 = num9 + Sigma_0(num11) + Maj(num11, num, num2);
		i++;
		num9 = num6 + Sigma_1(num12) + Ch(num12, num10, num5) + _K[(i)] + x[(i)];
		unsigned int num14 = num2 + num9;
		unsigned int num15 = num9 + Sigma_0(num13) + Maj(num13, num11, num);
		i++;
		num9 = num5 + Sigma_1(num14) + Ch(num14, num12, num10) + _K[(i)] + x[(i)];
		unsigned int num16 = num + num9;
		unsigned int num17 = num9 + Sigma_0(num15) + Maj(num15, num13, num11);
		i++;
		num9 = num10 + Sigma_1(num16) + Ch(num16, num14, num12) + _K[(i)] + x[(i)];
		num8 = num11 + num9;
		num4 = num9 + Sigma_0(num17) + Maj(num17, num15, num13);
		i++;
		num9 = num12 + Sigma_1(num8) + Ch(num8, num16, num14) + _K[(i)] + x[(i)];
		num7 = num13 + num9;
		num3 = num9 + Sigma_0(num4) + Maj(num4, num17, num15);
		i++;
		num9 = num14 + Sigma_1(num7) + Ch(num7, num8, num16) + _K[(i)] + x[(i)];
		num6 = num15 + num9;
		num2 = num9 + Sigma_0(num3) + Maj(num3, num4, num17);
		i++;
		num9 = num16 + Sigma_1(num6) + Ch(num6, num7, num8) + _K[(i)] + x[(i)];
		num5 = num17 + num9;
		num = num9 + Sigma_0(num2) + Maj(num2, num3, num4);
	}
	state[(0)] += num;
	state[(1)] += num2;
	state[(2)] += num3;
	state[(3)] += num4;
	state[(4)] += num5;
	state[(5)] += num6;
	state[(6)] += num7;
	state[(7)] += num8;
	return true;
}

__device__ unsigned int RotateRight(unsigned int x, int n)
{
	return x >> (n & 31) | (int)x << (32 - n & 31);
}

__device__ unsigned int Ch(unsigned int x, unsigned int y, unsigned int z)
{
	return (x & y) ^ ((x ^ 4294967295u) & z);
}

__device__ unsigned int Maj(unsigned int x, unsigned int y, unsigned int z)
{
	return (x & y) ^ (x & z) ^ (y & z);
}

__device__ unsigned int sigma_0(unsigned int x)
{
	return RotateRight(x, 7) ^ RotateRight(x, 18) ^ x >> 3;
}

__device__ unsigned int sigma_1(unsigned int x)
{
	return RotateRight(x, 17) ^ RotateRight(x, 19) ^ x >> 10;
}

__device__ unsigned int Sigma_0(unsigned int x)
{
	return RotateRight(x, 2) ^ RotateRight(x, 13) ^ RotateRight(x, 22);
}

__device__ unsigned int Sigma_1(unsigned int x)
{
	return RotateRight(x, 6) ^ RotateRight(x, 11) ^ RotateRight(x, 25);
}



__device__ void DWORDToBigEndian(unsigned char* block, unsigned int* x, int digits)
{
	int i = 0;
	int num = 0;
	while (i < digits)
	{
		block[(num)] = (unsigned char)(x[(i)] >> 24 & 0xff);
		block[(num + 1)] = (unsigned char)(x[(i)] >> 16 & 0xff);
		block[(num + 2)] = (unsigned char)(x[(i)] >> 8 & 0xff);
		block[(num + 3)] = (unsigned char)(x[(i)] & 0xff);
		i++;
		num += 4;
	}
}

__device__ void DWORDFromBigEndian(unsigned int* x, int xLen0, int digits, unsigned char* block, int blockLen0)
{
	int i = 0;
	int num = 0;
	while (i < digits)
	{
		x[(i)] = (unsigned int)((int)block[(num)] << 24 | (int)block[(num + 1)] << 16 | (int)block[(num + 2)] << 8 | block[(num + 3)]);
		i++;
		num += 4;
	}
}

__device__ void CopyArray(unsigned char* SourceArray, int SourceArrayLen0, int SourceIndex, unsigned char* DestinationArray, int DestinationArrayLen0, int DestinationIndex, int Length)
{
	for (int i = 0; i < Length; i++)
	{
		DestinationArray[(i + DestinationIndex)] = SourceArray[(i + SourceIndex)];
	}
}
int main(void)
{
	printf("Copyright (C) 2014 Sean Bradley\n\n");
	printf("Permission is hereby granted, free of charge, to any person obtaining a copy of this software and\nassociated documentation files (the 'Software'),\nto deal in the Software without restriction,\nincluding without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,\nand/or sell copies of the Software,\nand to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n");
	printf("The above copyright notice and this permission notice shall be included in all copies or substantialnportions of the Software.\n");
	printf("n\THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT\nLIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.\nIN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,\nWHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE\nSOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n");
	printf("n\SeanWasEre\n\n");

	int N = 65535;
	int block_size = 16;
	int n_blocks = N / block_size + (N%block_size == 0 ? 0 : 1);

	printf("%d %d\n", block_size, n_blocks);

	unsigned char *host_response, *dev_response;
	size_t size = 32 * sizeof(unsigned char);
	host_response = (unsigned char *)malloc(size);
	for (int i = 0; i < (int)size; i++)
	{
		host_response[i] = (unsigned char)0;
	}
	cudaMalloc((void **)&dev_response, size);
	cudaMemcpy(dev_response, host_response, size, cudaMemcpyHostToDevice);

	printf("response buffer size=%d\n", size);
	//'password' as sha356 hash = 5e 88 48 98 da 28 04 71 51 d0 e5 6f 8d c6 29 27 73 60 3d 0d 6a ab bd d6 2a 11 ef 72 1d 15 42 d8
	unsigned char *host_query, *dev_query;
	size = 32 * sizeof(unsigned char);
	host_query = (unsigned char *)malloc(size);
	host_query[0] = 0x5e;
	host_query[1] = 0x88;
	host_query[2] = 0x48;
	host_query[3] = 0x98;
	host_query[4] = 0xda;
	host_query[5] = 0x28;
	host_query[6] = 0x04;
	host_query[7] = 0x71;
	host_query[8] = 0x51;
	host_query[9] = 0xd0;
	host_query[10] = 0xe5;
	host_query[11] = 0x6f;
	host_query[12] = 0x8d;
	host_query[13] = 0xc6;
	host_query[14] = 0x29;
	host_query[15] = 0x27;
	host_query[16] = 0x73;
	host_query[17] = 0x60;
	host_query[18] = 0x3d;
	host_query[19] = 0x0d;
	host_query[20] = 0x6a;
	host_query[21] = 0xab;
	host_query[22] = 0xbd;
	host_query[23] = 0xd6;
	host_query[24] = 0x2a;
	host_query[25] = 0x11;
	host_query[26] = 0xef;
	host_query[27] = 0x72;
	host_query[28] = 0x1d;
	host_query[29] = 0x15;
	host_query[30] = 0x42;
	host_query[31] = 0xd8;
	cudaMalloc((void **)&dev_query, size);
	cudaMemcpy(dev_query, host_query, size, cudaMemcpyHostToDevice);

	std::clock_t start;

	start = std::clock();
	//kernel(host_response, 32, dev_query, 32, N);
	kernel <<< n_blocks, block_size >>> (dev_response, dev_query, 32, N);
	cudaMemcpy(host_response, dev_response, sizeof(unsigned char) * 32, cudaMemcpyDeviceToHost);
	printf("ms = %lu.nn", (std::clock() - start));

	cudaFree(dev_response); cudaFree(dev_query);

	for (int j = 0; j < 32; j++)
	{
		//printf("%02X", host_response[j + (i * 20)]);
		printf("%02X", host_response[j]);
	}
	printf("n");

	cudaError_t err = cudaGetLastError();
	if (err != cudaSuccess) printf("%sn", cudaGetErrorString(err));

	free(host_response);

	printf("\nCopyright SeanWasEre.com 2014\n\n");

	system("pause");

}
