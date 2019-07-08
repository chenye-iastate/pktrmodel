/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "X2AP-IEs"
 * 	found in "/home/matthias/openairinterface5g/openair2/X2AP/MESSAGES/ASN1/R11.2/X2AP-IEs.asn"
 * 	`asn1c -gen-PER`
 */

#include "X2ap-UsableABSInformation.h"

static asn_per_constraints_t asn_PER_type_X2ap_UsableABSInformation_constr_1 GCC_NOTUSED = {
	{ APC_CONSTRAINED | APC_EXTENSIBLE,  1,  1,  0,  1 }	/* (0..1,...) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_X2ap_UsableABSInformation_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct X2ap_UsableABSInformation, choice.fdd),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_X2ap_UsableABSInformationFDD,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"fdd"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct X2ap_UsableABSInformation, choice.tdd),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_X2ap_UsableABSInformationTDD,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"tdd"
		},
};
static asn_TYPE_tag2member_t asn_MAP_X2ap_UsableABSInformation_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* fdd at 1413 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* tdd at 1414 */
};
static asn_CHOICE_specifics_t asn_SPC_X2ap_UsableABSInformation_specs_1 = {
	sizeof(struct X2ap_UsableABSInformation),
	offsetof(struct X2ap_UsableABSInformation, _asn_ctx),
	offsetof(struct X2ap_UsableABSInformation, present),
	sizeof(((struct X2ap_UsableABSInformation *)0)->present),
	asn_MAP_X2ap_UsableABSInformation_tag2el_1,
	2,	/* Count of tags in the map */
	0,
	2	/* Extensions start */
};
asn_TYPE_descriptor_t asn_DEF_X2ap_UsableABSInformation = {
	"X2ap-UsableABSInformation",
	"X2ap-UsableABSInformation",
	CHOICE_free,
	CHOICE_print,
	CHOICE_constraint,
	CHOICE_decode_ber,
	CHOICE_encode_der,
	CHOICE_decode_xer,
	CHOICE_encode_xer,
	CHOICE_decode_uper,
	CHOICE_encode_uper,
	CHOICE_decode_aper,
	CHOICE_encode_aper,
	CHOICE_compare,
	CHOICE_outmost_tag,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	&asn_PER_type_X2ap_UsableABSInformation_constr_1,
	asn_MBR_X2ap_UsableABSInformation_1,
	2,	/* Elements count */
	&asn_SPC_X2ap_UsableABSInformation_specs_1	/* Additional specs */
};

