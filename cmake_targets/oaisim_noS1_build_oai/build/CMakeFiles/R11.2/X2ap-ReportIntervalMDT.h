/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "X2AP-IEs"
 * 	found in "/home/matthias/openairinterface5g/openair2/X2AP/MESSAGES/ASN1/R11.2/X2AP-IEs.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_X2ap_ReportIntervalMDT_H_
#define	_X2ap_ReportIntervalMDT_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum X2ap_ReportIntervalMDT {
	X2ap_ReportIntervalMDT_ms120	= 0,
	X2ap_ReportIntervalMDT_ms240	= 1,
	X2ap_ReportIntervalMDT_ms480	= 2,
	X2ap_ReportIntervalMDT_ms640	= 3,
	X2ap_ReportIntervalMDT_ms1024	= 4,
	X2ap_ReportIntervalMDT_ms2048	= 5,
	X2ap_ReportIntervalMDT_ms5120	= 6,
	X2ap_ReportIntervalMDT_ms10240	= 7,
	X2ap_ReportIntervalMDT_min1	= 8,
	X2ap_ReportIntervalMDT_min6	= 9,
	X2ap_ReportIntervalMDT_min12	= 10,
	X2ap_ReportIntervalMDT_min30	= 11,
	X2ap_ReportIntervalMDT_min60	= 12
} e_X2ap_ReportIntervalMDT;

/* X2ap-ReportIntervalMDT */
typedef long	 X2ap_ReportIntervalMDT_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_X2ap_ReportIntervalMDT;
asn_struct_free_f X2ap_ReportIntervalMDT_free;
asn_struct_print_f X2ap_ReportIntervalMDT_print;
asn_constr_check_f X2ap_ReportIntervalMDT_constraint;
ber_type_decoder_f X2ap_ReportIntervalMDT_decode_ber;
der_type_encoder_f X2ap_ReportIntervalMDT_encode_der;
xer_type_decoder_f X2ap_ReportIntervalMDT_decode_xer;
xer_type_encoder_f X2ap_ReportIntervalMDT_encode_xer;
per_type_decoder_f X2ap_ReportIntervalMDT_decode_uper;
per_type_encoder_f X2ap_ReportIntervalMDT_encode_uper;
per_type_decoder_f X2ap_ReportIntervalMDT_decode_aper;
per_type_encoder_f X2ap_ReportIntervalMDT_encode_aper;
type_compare_f     X2ap_ReportIntervalMDT_compare;

#ifdef __cplusplus
}
#endif

#endif	/* _X2ap_ReportIntervalMDT_H_ */
#include <asn_internal.h>
