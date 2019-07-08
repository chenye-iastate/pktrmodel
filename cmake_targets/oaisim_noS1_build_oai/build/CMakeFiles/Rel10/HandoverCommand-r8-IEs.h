/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "fixed_grammar.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_HandoverCommand_r8_IEs_H_
#define	_HandoverCommand_r8_IEs_H_


#include <asn_application.h>

/* Including external dependencies */
#include <OCTET_STRING.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* HandoverCommand-r8-IEs */
typedef struct HandoverCommand_r8_IEs {
	OCTET_STRING_t	 handoverCommandMessage;
	struct HandoverCommand_r8_IEs__nonCriticalExtension {
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *nonCriticalExtension;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} HandoverCommand_r8_IEs_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_HandoverCommand_r8_IEs;

#ifdef __cplusplus
}
#endif

#endif	/* _HandoverCommand_r8_IEs_H_ */
#include <asn_internal.h>
