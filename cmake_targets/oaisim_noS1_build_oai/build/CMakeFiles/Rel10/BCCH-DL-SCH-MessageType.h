/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "fixed_grammar.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_BCCH_DL_SCH_MessageType_H_
#define	_BCCH_DL_SCH_MessageType_H_


#include <asn_application.h>

/* Including external dependencies */
#include "SystemInformation.h"
#include "SystemInformationBlockType1.h"
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum BCCH_DL_SCH_MessageType_PR {
	BCCH_DL_SCH_MessageType_PR_NOTHING,	/* No components present */
	BCCH_DL_SCH_MessageType_PR_c1,
	BCCH_DL_SCH_MessageType_PR_messageClassExtension
} BCCH_DL_SCH_MessageType_PR;
typedef enum BCCH_DL_SCH_MessageType__c1_PR {
	BCCH_DL_SCH_MessageType__c1_PR_NOTHING,	/* No components present */
	BCCH_DL_SCH_MessageType__c1_PR_systemInformation,
	BCCH_DL_SCH_MessageType__c1_PR_systemInformationBlockType1
} BCCH_DL_SCH_MessageType__c1_PR;

/* BCCH-DL-SCH-MessageType */
typedef struct BCCH_DL_SCH_MessageType {
	BCCH_DL_SCH_MessageType_PR present;
	union BCCH_DL_SCH_MessageType_u {
		struct BCCH_DL_SCH_MessageType__c1 {
			BCCH_DL_SCH_MessageType__c1_PR present;
			union BCCH_DL_SCH_MessageType__c1_u {
				SystemInformation_t	 systemInformation;
				SystemInformationBlockType1_t	 systemInformationBlockType1;
			} choice;
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} c1;
		struct BCCH_DL_SCH_MessageType__messageClassExtension {
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} messageClassExtension;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} BCCH_DL_SCH_MessageType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_BCCH_DL_SCH_MessageType;

#ifdef __cplusplus
}
#endif

#endif	/* _BCCH_DL_SCH_MessageType_H_ */
#include <asn_internal.h>
