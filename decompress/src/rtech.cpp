#include "rtech.h"

/******************************************************************************
-------------------------------------------------------------------------------
File   : rtech.cpp
Date   : 18:07:2021
Author : Kawe Mazidjatari
Purpose: Implements the 'rtech_game' core utilities
-------------------------------------------------------------------------------
History:
- 18:07:2021 | 13:02 : Created by Kawe Mazidjatari
- 10:09:2021 | 18:22 : Implement 'StringToGuid' method
- 12:11:2021 | 14:41 : Add decompression method to ConCommand callback

******************************************************************************/

//-----------------------------------------------------------------------------
// Purpose: calculate 'decompressed' size and commit parameters
//-----------------------------------------------------------------------------
uint64_t RTech::DecompressPakfileInit(RpakDecompressionState *state, uint8_t *file_buffer, int64_t file_size, int64_t off_no_header, int64_t header_size)
{
	int64_t input_byte_pos_init;		// r9
	uint64_t byte_init;					// r11
	int32_t decompressed_size_bits;		// ecx
	int64_t byte_1_low;					// rdi
	uint64_t input_byte_pos_1;			// r10
	uint32_t bit_pos_final;				// ebp
	uint64_t byte_1;					// rdi
	uint32_t brih_bits;					// er11
	uint64_t inv_mask_in;				// r8
	uint64_t byte_final_full;			// rbx
	uint64_t bit_pos_final_1;			// rax
	int32_t byte_bit_offset_final;		// ebp
	uint64_t input_byte_pos_final;		// r10
	uint64_t byte_final;				// rbx
	uint32_t brih_bytes;				// er11
	int64_t byte_tmp;					// rdx
	int64_t stream_len_needed;			// r14
	int64_t result;						// rax
	uint64_t inv_mask_out;				// r8
	int64_t qw70;						// rcx
	int64_t stream_compressed_size_new; // rdx

	const uintptr_t mask = UINT64_MAX;
	const uintptr_t file_buf = uintptr_t(file_buffer);

	state->input_buf = file_buf;
	state->out = 0LL;
	state->out_mask = 0LL;
	state->dword44 = 0; // y r u gay
	state->file_len_total = file_size + off_no_header;
	state->mask = mask;
	input_byte_pos_init = off_no_header + header_size + 8;
	byte_init = *(uint64_t *)((mask & (off_no_header + header_size)) + file_buf);
	state->decompressed_position = header_size;
	decompressed_size_bits = byte_init & 0x3F;
	byte_init >>= 6;
	state->input_byte_pos = input_byte_pos_init;
	state->decompressed_size = byte_init & ((1LL << decompressed_size_bits) - 1) | (1LL << decompressed_size_bits);
	byte_1_low = *(uint64_t *)((mask & input_byte_pos_init) + file_buf) << (64 - ((uint8_t)decompressed_size_bits + 6));
	input_byte_pos_1 = input_byte_pos_init + ((uint64_t)(uint32_t)(decompressed_size_bits + 6) >> 3);
	state->input_byte_pos = input_byte_pos_1;
	bit_pos_final = ((decompressed_size_bits + 6) & 7) + 13;
	byte_1 = (0xFFFFFFFFFFFFFFFFuLL >> ((decompressed_size_bits + 6) & 7)) & ((byte_init >> decompressed_size_bits) | byte_1_low);
	brih_bits = (((uint8_t)byte_1 - 1) & 0x3F) + 1;
	inv_mask_in = 0xFFFFFFFFFFFFFFFFuLL >> (64 - (uint8_t)brih_bits);
	state->inv_mask_in = inv_mask_in;
	state->inv_mask_out = 0xFFFFFFFFFFFFFFFFuLL >> (63 - (((byte_1 >> 6) - 1) & 0x3F));
	byte_final_full = (byte_1 >> 13) | (*(uint64_t *)((mask & input_byte_pos_1) + file_buf) << (64 - (uint8_t)bit_pos_final));
	bit_pos_final_1 = bit_pos_final;
	byte_bit_offset_final = bit_pos_final & 7;
	input_byte_pos_final = (bit_pos_final_1 >> 3) + input_byte_pos_1;
	byte_final = (0xFFFFFFFFFFFFFFFFuLL >> byte_bit_offset_final) & byte_final_full;
	state->input_byte_pos = input_byte_pos_final;
	if (inv_mask_in == -1LL)
	{
		state->header_skip_bytes_bs = 0;
		stream_len_needed = file_size;
	}
	else
	{
		brih_bytes = brih_bits >> 3;
		state->header_skip_bytes_bs = brih_bytes + 1;
		byte_tmp = *(uint64_t *)((mask & input_byte_pos_final) + file_buf);
		state->input_byte_pos = input_byte_pos_final + brih_bytes + 1;
		stream_len_needed = byte_tmp & ((1LL << (8 * ((uint8_t)brih_bytes + 1))) - 1);
	}
	result = state->decompressed_size;
	inv_mask_out = state->inv_mask_out;
	qw70 = off_no_header + state->inv_mask_in - 6LL;
	state->len_needed = stream_len_needed + off_no_header;
	state->qword70 = qw70;
	state->byte = byte_final;
	state->byte_bit_offset = byte_bit_offset_final;
	state->dword6C = 0;
	state->stream_compressed_size = stream_len_needed + off_no_header;
	state->stream_decompressed_size = result;
	if (result - 1 > inv_mask_out)
	{
		stream_compressed_size_new = stream_len_needed + off_no_header - state->header_skip_bytes_bs;
		state->stream_decompressed_size = inv_mask_out + 1;
		state->stream_compressed_size = stream_compressed_size_new;
	}
	return result;
}

//-----------------------------------------------------------------------------
// Purpose: decompress input pakfile
//-----------------------------------------------------------------------------
uint8_t RTech::DecompressPakfile(RpakDecompressionState *state, uint64_t inLen, uint64_t outLen)
{
	uint64_t decompressed_position;		   // r15
	uint32_t byte_bit_offset;			   // ebp
	uint64_t byte;						   // rsi
	uint64_t input_byte_pos;			   // rdi
	uint64_t some_size;					   // r12
	uint32_t dword6C;					   // ecx MAPDST
	uint64_t v12;						   // rsi
	uint64_t i;							   // rax
	uint64_t dword6c_shl8;				   // r8
	int64_t dword6c_old;				   // r9
	int32_t LUT_200_val;				   // ecx
	uint64_t v17;						   // rax
	uint64_t byte_new;					   // rsi
	int64_t LUT_0_VAL;					   // r14
	int32_t byte_4bits_1;				   // ecx
	uint64_t v21;						   // r11
	int32_t v22;						   // edx
	uint64_t out_mask;					   // rax
	int32_t v24;						   // er8
	uint32_t LUT_400_seek_backwards;	   // er13
	uint64_t out_seek_back;				   // r10
	uint64_t out_seekd_1;				   // rax
	uint64_t *out_seekd_back;			   // r10
	uint64_t decompressed_size;			   // r9
	uint64_t inv_mask_in;				   // r10
	uint64_t header_skip_bytes_bs;		   // r8
	uint64_t v32;						   // rax
	uint64_t v33;						   // rax
	uint64_t v34;						   // rax
	uint64_t stream_decompressed_size_new; // rcx
	int64_t v36;						   // rdx
	uint64_t len_needed_new;			   // r14
	uint64_t stream_compressed_size_new;   // r11
	char v39;							   // cl MAPDST
	uint64_t v40;						   // rsi MAPDST
	uint64_t v46;						   // rcx
	int64_t v47;						   // r9
	int64_t m;							   // r8
	uint32_t v49;						   // er9
	int64_t v50;						   // r8
	int64_t v51;						   // rdx
	int64_t k;							   // r8
	char *v53;							   // r10
	int64_t v54;						   // rdx
	uint32_t lut0_val_abs;				   // er14
	int64_t *in_seekd;					   // rdx
	int64_t *out_seekd;					   // r8
	int64_t byte_3bits;					   // rax MAPDST
	uint64_t byte_new_tmp;				   // r9 MAPDST
	int32_t LUT_4D0_480;				   // er10 MAPDST
	uint8_t LUT_4D8_4C0_nBits;			   // cl MAPDST
	uint64_t byte_4bits;				   // rax MAPDST
	uint32_t copy_bytes_ammount;		   // er14
	uint32_t j;							   // ecx
	int64_t v67;						   // rax
	uint64_t v68;						   // rcx
	uint8_t result;						   // al

	if (inLen < state->len_needed)
		return 0;

	decompressed_position = state->decompressed_position;
	if (outLen < state->inv_mask_out + (decompressed_position & ~state->inv_mask_out) + 1 && outLen < state->decompressed_size)
	{
		return 0;
	}

	byte_bit_offset = state->byte_bit_offset; // Keeping copy since we increment it down below.
	byte = state->byte;						  // Keeping copy since its getting overwritten down below.
	input_byte_pos = state->input_byte_pos;	  // Keeping copy since we increment it down below.
	some_size = state->qword70;
	if (state->stream_compressed_size < some_size)
		some_size = state->stream_compressed_size;
	dword6C = state->dword6C;

	if (!byte_bit_offset)
		goto LABEL_9;

	v12 = (*(uint64_t *)((input_byte_pos & state->mask) + state->input_buf) << (64 - (uint8_t)byte_bit_offset)) | byte;
	for (i = byte_bit_offset;; i = byte_bit_offset)
	{
		byte_bit_offset &= 7u;
		input_byte_pos += i >> 3;
		byte = (0xFFFFFFFFFFFFFFFFuLL >> byte_bit_offset) & v12;
	LABEL_9:
		dword6c_shl8 = (uint64_t)dword6C << 8;
		dword6c_old = dword6C;
		LUT_200_val = LUT_200[(uint8_t)byte + dword6c_shl8]; // LUT_200 - u8 - ammount of bits
		v17 = (uint8_t)byte + dword6c_shl8;
		byte_bit_offset += LUT_200_val;
		byte_new = byte >> LUT_200_val;
		LUT_0_VAL = LUT_0[v17]; // LUT_0 - i32 - signed, ammount of bytes

		if (LUT_0_VAL < 0)
		{
			lut0_val_abs = -(int32_t)LUT_0_VAL;
			in_seekd = (int64_t *)(state->input_buf + (input_byte_pos & state->mask));
			dword6C = 1;
			out_seekd = (int64_t *)(state->out + (decompressed_position & state->out_mask));
			if (lut0_val_abs == LUT_4E0[dword6c_old])
			{
				if ((~input_byte_pos & state->inv_mask_in) < 0xF || (state->inv_mask_out & ~decompressed_position) < 0xF || state->decompressed_size - decompressed_position < 0x10)
				{
					lut0_val_abs = 1;
				}

				v39 = byte_new;
				v40 = byte_new >> 3;
				byte_3bits = v39 & 7;
				byte_new_tmp = v40;

				if (byte_3bits)
				{
					LUT_4D0_480 = LUT_4D0[byte_3bits];		 // LUT_4D0 - u8
					LUT_4D8_4C0_nBits = LUT_4D8[byte_3bits]; // LUT_4D8 - u8 - ammount of bits
				}
				else
				{
					byte_new_tmp = v40 >> 4;
					byte_4bits = v40 & 15;
					byte_bit_offset += 4;
					LUT_4D0_480 = LUT_480[byte_4bits];		 // LUT_480 - u32
					LUT_4D8_4C0_nBits = LUT_4C0[byte_4bits]; // LUT_4C0 - u8 - ammount of bits???
				}

				byte_bit_offset += LUT_4D8_4C0_nBits + 3;
				byte_new = byte_new_tmp >> LUT_4D8_4C0_nBits;
				copy_bytes_ammount = LUT_4D0_480 + (byte_new_tmp & ((1 << LUT_4D8_4C0_nBits) - 1)) + lut0_val_abs;

				for (j = copy_bytes_ammount >> 3; j; --j) // copy by 8 bytes
				{
					v67 = *in_seekd++;
					*out_seekd++ = v67;
				}

				if ((copy_bytes_ammount & 4) != 0) // copy by 4
				{
					*(uint32_t *)out_seekd = *(uint32_t *)in_seekd;
					out_seekd = (int64_t *)((char *)out_seekd + 4);
					in_seekd = (int64_t *)((char *)in_seekd + 4);
				}

				if ((copy_bytes_ammount & 2) != 0) // copy by 2
				{
					*(uint16_t *)out_seekd = *(uint16_t *)in_seekd;
					out_seekd = (int64_t *)((char *)out_seekd + 2);
					in_seekd = (int64_t *)((char *)in_seekd + 2);
				}

				if ((copy_bytes_ammount & 1) != 0) // copy by 1
					*(uint8_t *)out_seekd = *(uint8_t *)in_seekd;

				input_byte_pos += copy_bytes_ammount;
				decompressed_position += copy_bytes_ammount;
			}
			else
			{
				*out_seekd = *in_seekd;
				out_seekd[1] = in_seekd[1];
				input_byte_pos += lut0_val_abs;
				decompressed_position += lut0_val_abs;
			}
		}
		else
		{
			byte_4bits_1 = byte_new & 0xF;
			dword6C = 0;
			v21 = ((uint64_t)(uint32_t)byte_new >> (((uint32_t)(byte_4bits_1 + 0xFFFFFFE1) >> 3) & 6)) & 0x3F;				  // 6 bits after shift for who knows how much???
			v22 = 1 << (byte_4bits_1 + ((byte_new >> 4) & ((24 * (((uint32_t)(byte_4bits_1 + 0xFFFFFFE1) >> 3) & 2)) >> 4))); // ammount of bits to read???
			byte_bit_offset += (((uint32_t)(byte_4bits_1 + 0xFFFFFFE1) >> 3) & 6)											  // shit shit gets shifted by ammount of bits it read or something
							   + LUT_440[v21] + byte_4bits_1 + ((byte_new >> 4) & ((24 * (((uint32_t)(byte_4bits_1 + 0xFFFFFFE1) >> 3) & 2)) >> 4));
			out_mask = state->out_mask;
			v24 = 16 * (v22 + ((v22 - 1) & (byte_new >> ((((uint32_t)(byte_4bits_1 + 0xFFFFFFE1) >> 3) & 6) + LUT_440[v21]))));
			byte_new >>= (((uint32_t)(byte_4bits_1 + 0xFFFFFFE1) >> 3) & 6) + LUT_440[v21] + byte_4bits_1 + ((byte_new >> 4) & ((24 * (((uint32_t)(byte_4bits_1 + 0xFFFFFFE1) >> 3) & 2)) >> 4));
			LUT_400_seek_backwards = v24 + LUT_400[v21] - 16; // LUT_400 - u8 - seek backwards
			out_seek_back = out_mask & (decompressed_position - LUT_400_seek_backwards);
			out_seekd_1 = state->out + (decompressed_position & out_mask);
			out_seekd_back = (uint64_t *)(state->out + out_seek_back);
			if ((int32_t)LUT_0_VAL == 17)
			{
				v39 = byte_new;
				v40 = byte_new >> 3;
				byte_3bits = v39 & 7;
				byte_new_tmp = v40;
				if (byte_3bits)
				{
					LUT_4D0_480 = LUT_4D0[byte_3bits];
					LUT_4D8_4C0_nBits = LUT_4D8[byte_3bits];
				}
				else
				{
					byte_bit_offset += 4;
					byte_4bits = v40 & 0xF;
					byte_new_tmp = v40 >> 4;
					LUT_4D0_480 = LUT_480[byte_4bits];
					LUT_4D8_4C0_nBits = LUT_4C0[byte_4bits];
					if (state->input_buf && byte_bit_offset + LUT_4D8_4C0_nBits >= 0x3D)
					{
						v46 = input_byte_pos++ & state->mask;
						byte_new_tmp |= (uint64_t) * (uint8_t *)(v46 + state->input_buf) << (61 - (uint8_t)byte_bit_offset);
						byte_bit_offset -= 8;
					}
				}
				byte_bit_offset += LUT_4D8_4C0_nBits + 3;
				byte_new = byte_new_tmp >> LUT_4D8_4C0_nBits;
				v47 = ((uint32_t)byte_new_tmp & ((1 << LUT_4D8_4C0_nBits) - 1)) + LUT_4D0_480 + 17;
				decompressed_position += v47;
				if (LUT_400_seek_backwards < 8)
				{
					v49 = v47 - 13;
					decompressed_position -= 13LL;
					if (LUT_400_seek_backwards == 1) // 1 means copy v49 qwords?
					{
						v50 = *(uint8_t *)out_seekd_back;
						v51 = 0LL;
						for (k = 0x101010101010101LL * v50; (uint32_t)v51 < v49; v51 = (uint32_t)(v51 + 8))
							*(uint64_t *)(v51 + out_seekd_1) = k;
					}
					else
					{
						if (v49)
						{
							v53 = (char *)out_seekd_back - out_seekd_1;
							v54 = v49;
							do
							{
								*(uint8_t *)out_seekd_1 = v53[out_seekd_1]; // seekd = seek_back; increment ptrs
								++out_seekd_1;
								--v54;
							} while (v54);
						}
					}
				}
				else
				{
					for (m = 0LL; (uint32_t)m < (uint32_t)v47; m = (uint32_t)(m + 8))
						*(uint64_t *)(m + out_seekd_1) = *(uint64_t *)((char *)out_seekd_back + m);
				}
			}
			else
			{
				decompressed_position += LUT_0_VAL;
				*(uint64_t *)out_seekd_1 = *out_seekd_back;
				*(uint64_t *)(out_seekd_1 + 8) = out_seekd_back[1];
			}
		}
		if (input_byte_pos >= some_size)
			break;

	LABEL_26:
		v12 = (*(uint64_t *)((input_byte_pos & state->mask) + state->input_buf) << (64 - (uint8_t)byte_bit_offset)) | byte_new;
	}

	if (decompressed_position != state->stream_decompressed_size)
		goto LABEL_22;

	decompressed_size = state->decompressed_size;
	if (decompressed_position == decompressed_size)
	{
		state->input_byte_pos = input_byte_pos;
		result = 1;
		state->decompressed_position = decompressed_position;
		return result;
	}

	inv_mask_in = state->inv_mask_in;
	header_skip_bytes_bs = state->header_skip_bytes_bs;
	v32 = inv_mask_in & -(int64_t)input_byte_pos;
	byte_new >>= 1;
	++byte_bit_offset;

	if (header_skip_bytes_bs > v32)
	{
		input_byte_pos += v32;
		v33 = state->qword70;
		if (input_byte_pos > v33)
			state->qword70 = inv_mask_in + v33 + 1;
	}

	v34 = input_byte_pos & state->mask;
	input_byte_pos += header_skip_bytes_bs;
	stream_decompressed_size_new = decompressed_position + state->inv_mask_out + 1;
	v36 = *(uint64_t *)(v34 + state->input_buf) & ((1LL << (8 * (uint8_t)header_skip_bytes_bs)) - 1);
	len_needed_new = v36 + state->len_needed;
	stream_compressed_size_new = v36 + state->stream_compressed_size;
	state->len_needed = len_needed_new;
	state->stream_compressed_size = stream_compressed_size_new;

	if (stream_decompressed_size_new >= decompressed_size)
	{
		stream_decompressed_size_new = decompressed_size;
		state->stream_compressed_size = header_skip_bytes_bs + stream_compressed_size_new;
	}

	state->stream_decompressed_size = stream_decompressed_size_new;

	if (inLen >= len_needed_new && outLen >= stream_decompressed_size_new)
	{
	LABEL_22:
		some_size = state->qword70;
		if (input_byte_pos >= some_size)
		{
			input_byte_pos = ~state->inv_mask_in & (input_byte_pos + 7);
			some_size += state->inv_mask_in + 1;
			state->qword70 = some_size;
		}
		if (state->stream_compressed_size < some_size)
			some_size = state->stream_compressed_size;
		goto LABEL_26;
	}

	v68 = state->qword70;

	if (input_byte_pos >= v68)
	{
		input_byte_pos = ~inv_mask_in & (input_byte_pos + 7);
		state->qword70 = v68 + inv_mask_in + 1;
	}

	state->dword6C = dword6C;
	result = 0;
	state->input_byte_pos = input_byte_pos;
	state->decompressed_position = decompressed_position;
	state->byte = byte_new;
	state->byte_bit_offset = byte_bit_offset;

	return result;
}

int _BitScanReverseCrossplatform(int v12, uint32_t v10)
{
	// Determine the number of bits in an unsigned int
	int numBits = std::numeric_limits<unsigned int>::digits;

	// Find the position of the most significant set bit
	for (v12 = numBits - 1; v12 >= 0; --v12)
	{
		if ((v10 & (1u << v12)) != 0)
		{
			break; // Found the most significant set bit
		}
	}

	return v12;
}

//-----------------------------------------------------------------------------
// Purpose: calculate 'GUID' from input data
//-----------------------------------------------------------------------------
uint64_t RTech::StringToGuid(const char *asset_name)
{
	uint64_t v2;  // r10
	int v3;		  // er11
	uint32_t v4;  // er9
	uint32_t i;	  // edx
	uint64_t v6;  // rcx
	int v7;		  // er9
	int v8;		  // edx
	int v9;		  // eax
	uint32_t v10; // er8
	int v12;	  // ecx
	uint32_t *a1 = (uint32_t *)asset_name;

	v2 = 0LL;
	v3 = 0;
	v4 = (*a1 - 45 * ((~(*a1 ^ 0x5C5C5C5Cu) >> 7) & (((*a1 ^ 0x5C5C5C5Cu) - 0x1010101) >> 7) & 0x1010101)) & 0xDFDFDFDF;
	for (i = ~*a1 & (*a1 - 0x1010101) & 0x80808080; !i; i = v8 & 0x80808080)
	{
		v6 = v4;
		v7 = a1[1];
		++a1;
		v3 += 4;
		v2 = ((((uint64_t)(0xFB8C4D96501LL * v6) >> 24) + 0x633D5F1 * v2) >> 61) ^ (((uint64_t)(0xFB8C4D96501LL * v6) >> 24) + 0x633D5F1 * v2);
		v8 = ~v7 & (v7 - 0x1010101);
		v4 = (v7 - 45 * ((~(v7 ^ 0x5C5C5C5Cu) >> 7) & (((v7 ^ 0x5C5C5C5Cu) - 0x1010101) >> 7) & 0x1010101)) & 0xDFDFDFDF;
	}
	v9 = -1;
	v10 = (i & -(signed)i) - 1;

	if (_BitScanReverseCrossplatform(v12, v10))
	{
		v9 = v12;
	}
	return 0x633D5F1 * v2 + ((0xFB8C4D96501LL * (uint64_t)(v4 & v10)) >> 24) - 0xAE502812AA7333LL * (uint32_t)(v3 + v9 / 8);
}
///////////////////////////////////////////////////////////////////////////////
