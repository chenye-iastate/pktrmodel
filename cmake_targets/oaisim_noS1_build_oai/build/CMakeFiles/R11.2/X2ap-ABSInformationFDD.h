/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "X2AP-IEs"
 * 	found in "/home/matthias/openairinterface5g/openair2/X2AP/MESSAGES/ASN1/R11.2/X2AP-IEs.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_X2ap_ABSInformationFDD_H_
#define	_X2ap_ABSInformationFDD_H_


#include <asn_application.h>

/* Including external dependencies */
#include <BIT_STRING.h>
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum X2ap_ABSInformationFDD__numberOfCellSpecificAntennaPorts {
	X2ap_ABSInformationFDD__numberOfCellSpecificAntennaPorts_one	= 0,
	X2ap_ABSInformationFDD__numberOfCellSpecificAntennaPorts_two	= 1,
	X2ap_ABSInformationFDD__numberOfCellSpecificAntennaPorts_four	= 2
	/*
	 * Enumeration is extensible
	 */
} e_X2ap_ABSInformationFDD__numberOfCellSpecificAntennaPorts;

/* Forward declarations */
struct X2ap_IE_Extensions;

/* X2ap-ABSInformationFDD */
typedef struct X2ap_ABSInformationFDD {
	BIT_STRING_t	 abs_pattern_info;
	long	 numberOfCellSpecificAntennaPorts;
	BIT_STRING_t	 measurement_subset;
	struct X2ap_IE_Extensions	*iE_Extensions	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} X2ap_ABSInformationFDD_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_numberOfCellSpecificAntennaPorts_3;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_X2ap_ABSInformationFDD;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "X2ap-IE-Extensions.h"

#endif	/* _X2ap_ABSInformationFDD_H_ */
#include <asn_internal.h>
