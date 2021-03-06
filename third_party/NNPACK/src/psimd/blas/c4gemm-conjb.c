#include <stddef.h>
#include <stdint.h>

#include <psimd.h>


void nnp_c4gemm_conjb_only_2x2__psimd(
	size_t k, size_t update,
	const float a[restrict static 1],
	const float b[restrict static 1],
	float c[restrict static 1],
	size_t row_stride_c)
{
	psimd_f32 acc00r = psimd_zero_f32(), acc00i = psimd_zero_f32();
	psimd_f32 acc01r = psimd_zero_f32(), acc01i = psimd_zero_f32();
	psimd_f32 acc10r = psimd_zero_f32(), acc10i = psimd_zero_f32();
	psimd_f32 acc11r = psimd_zero_f32(), acc11i = psimd_zero_f32();
	do {
		const psimd_f32 a0r = psimd_load_f32(a +  0);
		const psimd_f32 a0i = psimd_load_f32(a +  4);
		const psimd_f32 a1r = psimd_load_f32(a +  8);
		const psimd_f32 a1i = psimd_load_f32(a + 12);

		const psimd_f32 b0r = psimd_load_f32(b + 0);
		const psimd_f32 b1r = psimd_load_f32(b + 8);
		acc00r += a0r * b0r;
		acc00i += a0i * b0r;
		acc01r += a0r * b1r;
		acc01i += a0i * b1r;
		acc10r += a1r * b0r;
		acc10i += a1i * b0r;
		acc11r += a1r * b1r;
		acc11i += a1i * b1r;
		const psimd_f32 b0i = psimd_load_f32(b +  4);
		const psimd_f32 b1i = psimd_load_f32(b + 12);
		acc00r += a0i * b0i;
		acc00i -= a0r * b0i;
		acc01r += a0i * b1i;
		acc01i -= a0r * b1i;
		acc10r += a1i * b0i;
		acc10i -= a1r * b0i;
		acc11r += a1i * b1i;
		acc11i -= a1r * b1i;

		a += 16;
		b += 16;
	} while (--k);

	if (update != 0) {
		psimd_store_f32(c +  0, psimd_load_f32(c +  0) + acc00r);
		psimd_store_f32(c +  4, psimd_load_f32(c +  4) + acc00i);
		psimd_store_f32(c +  8, psimd_load_f32(c +  8) + acc01r);
		psimd_store_f32(c + 12, psimd_load_f32(c + 12) + acc01i);
		c += row_stride_c;
		psimd_store_f32(c +  0, psimd_load_f32(c +  0) + acc10r);
		psimd_store_f32(c +  4, psimd_load_f32(c +  4) + acc10i);
		psimd_store_f32(c +  8, psimd_load_f32(c +  8) + acc11r);
		psimd_store_f32(c + 12, psimd_load_f32(c + 12) + acc11i);
	} else {
		psimd_store_f32(c +  0, acc00r);
		psimd_store_f32(c +  4, acc00i);
		psimd_store_f32(c +  8, acc01r);
		psimd_store_f32(c + 12, acc01i);
		c += row_stride_c;
		psimd_store_f32(c +  0, acc10r);
		psimd_store_f32(c +  4, acc10i);
		psimd_store_f32(c +  8, acc11r);
		psimd_store_f32(c + 12, acc11i);
	}
}

void nnp_c4gemm_conjb_upto_2x2__psimd(
	uint32_t mr, uint32_t nr,
	size_t k, size_t update,
	const float a[restrict static 1],
	const float b[restrict static 1],
	float c[restrict static 1],
	size_t row_stride_c)
{
	psimd_f32 acc00r = psimd_zero_f32(), acc00i = psimd_zero_f32();
	psimd_f32 acc01r = psimd_zero_f32(), acc01i = psimd_zero_f32();
	psimd_f32 acc10r = psimd_zero_f32(), acc10i = psimd_zero_f32();
	psimd_f32 acc11r = psimd_zero_f32(), acc11i = psimd_zero_f32();
	do {
		psimd_f32 a0r, a0i, a1r, a1i;

		a0r = psimd_load_f32(a + 0);
		a0i = psimd_load_f32(a + 4);
		a += 8;
		if (mr > 1) {
			a1r = psimd_load_f32(a + 0);
			a1i = psimd_load_f32(a + 4);
			a += 8;
		}

		const psimd_f32 b0r = psimd_load_f32(b + 0);
		const psimd_f32 b0i = psimd_load_f32(b + 4);
		b += 8;

		acc00r += a0r * b0r;
		acc00i += a0i * b0r;
		acc10r += a1r * b0r;
		acc10i += a1i * b0r;

		acc00r += a0i * b0i;
		acc00i -= a0r * b0i;
		acc10r += a1i * b0i;
		acc10i -= a1r * b0i;

		if (nr > 1) {
			const psimd_f32 b1r = psimd_load_f32(b + 0);
			const psimd_f32 b1i = psimd_load_f32(b + 4);
			b += 8;

			acc01r += a0r * b1r;
			acc01i += a0i * b1r;
			acc11r += a1r * b1r;
			acc11i += a1i * b1r;

			acc01r += a0i * b1i;
			acc01i -= a0r * b1i;
			acc11r += a1i * b1i;
			acc11i -= a1r * b1i;
		}
	} while (--k);

	if (update != 0) {
		psimd_store_f32(c + 0, psimd_load_f32(c + 0) + acc00r);
		psimd_store_f32(c + 4, psimd_load_f32(c + 4) + acc00i);
		if (nr > 1) {
			psimd_store_f32(c +  8, psimd_load_f32(c +  8) + acc01r);
			psimd_store_f32(c + 12, psimd_load_f32(c + 12) + acc01i);
		}
		if (mr > 1) {
			c += row_stride_c;
			psimd_store_f32(c +  0, psimd_load_f32(c +  0) + acc10r);
			psimd_store_f32(c +  4, psimd_load_f32(c +  4) + acc10i);
			if (nr > 1) {
				psimd_store_f32(c +  8, psimd_load_f32(c +  8) + acc11r);
				psimd_store_f32(c + 12, psimd_load_f32(c + 12) + acc11i);
			}
		}
	} else {
		psimd_store_f32(c + 0, acc00r);
		psimd_store_f32(c + 4, acc00i);
		if (nr > 1) {
			psimd_store_f32(c +  8, acc01r);
			psimd_store_f32(c + 12, acc01i);
		}
		if (mr > 1) {
			c += row_stride_c;
			psimd_store_f32(c + 0, acc10r);
			psimd_store_f32(c + 4, acc10i);
			if (nr > 1) {
				psimd_store_f32(c +  8, acc11r);
				psimd_store_f32(c + 12, acc11i);
			}
		}
	}
}
