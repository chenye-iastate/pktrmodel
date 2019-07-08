/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "X2AP-IEs"
 * 	found in "/home/matthias/openairinterface5g/openair2/X2AP/MESSAGES/ASN1/R11.2/X2AP-IEs.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_X2ap_EUTRA_Mode_Info_H_
#define	_X2ap_EUTRA_Mode_Info_H_


#include <asn_application.h>

/* Including external dependencies */
#include "X2ap-FDD-Info.h"
#include "X2ap-TDD-Info.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum X2ap_EUTRA_Mode_Info_PR {
	X2ap_EUTRA_Mode_Info_PR_NOTHING,	/* No components present */
	X2ap_EUTRA_Mode_Info_PR_fDD,
	X2ap_EUTRA_Mode_Info_PR_tDD,
	/* Extensions may appear below */
	
} X2ap_EUTRA_Mode_Info_PR;

/* X2ap-EUTRA-Mode-Info */
typedef struct X2ap_EUTRA_Mode_Info {
	X2ap_EUTRA_Mode_Info_PR present;
	union X2ap_EUTRA_Mode_Info_u {
		X2ap_FDD_Info_t	 fDD;
		X2ap_TDD_Info_t	 tDD;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} X2ap_EUTRA_Mode_Info_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_X2ap_EUTRA_Mode_Info;

#ifdef __cplusplus
}
#endif

#endif	/* _X2ap_EUTRA_Mode_Info_H_ */
#include <asn_internal.h>
