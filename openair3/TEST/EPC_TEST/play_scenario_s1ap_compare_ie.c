/*******************************************************************************
    OpenAirInterface
    Copyright(c) 1999 - 2014 Eurecom

    OpenAirInterface is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.


    OpenAirInterface is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with OpenAirInterface.The full GNU General Public License is
    included in this distribution in the file called "COPYING". If not,
    see <http://www.gnu.org/licenses/>.

  Contact Information
  OpenAirInterface Admin: openair_admin@eurecom.fr
  OpenAirInterface Tech : openair_tech@eurecom.fr
  OpenAirInterface Dev  : openair4g-devel@lists.eurecom.fr

  Address      : Eurecom, Campus SophiaTech, 450 Route des Chappes, CS 50193 - 06904 Biot Sophia Antipolis cedex, FRANCE

 *******************************************************************************/

/*
                                play_scenario_s1ap_compare_ie.c
                                -------------------
  AUTHOR  : Lionel GAUTHIER
  COMPANY : EURECOM
  EMAIL   : Lionel.Gauthier@eurecom.fr
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <unistd.h>
#include <crypt.h>
#include <errno.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include "tree.h"
#include "queue.h"


#include "intertask_interface.h"
#include "timer.h"
#include "platform_types.h"
#include "assertions.h"
#include "conversions.h"
#include "s1ap_common.h"
#include "play_scenario_s1ap_eNB_defs.h"
#include "play_scenario.h"
#include "msc.h"
//------------------------------------------------------------------------------
extern et_scenario_t  *g_scenario;
extern uint32_t        g_constraints;
//------------------------------------------------------------------------------

asn_comp_rval_t * et_s1ap_ies_is_matching(const S1AP_PDU_PR present, s1ap_message * const m1, s1ap_message * const m2, const uint32_t constraints)
{
  asn_comp_rval_t *ret = NULL;
  AssertFatal(m1 != NULL, "bad parameter m1");
  AssertFatal(m2 != NULL, "bad parameter m2");
  AssertFatal((present == S1AP_PDU_PR_initiatingMessage) ||
              (present == S1AP_PDU_PR_successfulOutcome) ||
              (present == S1AP_PDU_PR_unsuccessfulOutcome) , "Bad parameter S1AP_PDU_PR present ");
  AssertFatal( m1->procedureCode == m2->procedureCode, "Bad parameters: no matching procedure codes");


  // some cases can never occur since uplink only.
  switch (m1->procedureCode) {
    case  S1ap_ProcedureCode_id_HandoverPreparation:
      if (present == S1AP_PDU_PR_initiatingMessage) {
        ret = s1ap_compare_s1ap_handoverrequiredies(
                &m1->msg.s1ap_HandoverRequiredIEs,
                &m2->msg.s1ap_HandoverRequiredIEs);
      } else {
        ret = s1ap_compare_s1ap_handovercommandies(
                &m1->msg.s1ap_HandoverCommandIEs,
                &m2->msg.s1ap_HandoverCommandIEs);
      }
      break;

    case  S1ap_ProcedureCode_id_HandoverResourceAllocation:
      if (present == S1AP_PDU_PR_initiatingMessage) {
        ret = s1ap_compare_s1ap_handoverrequesties(
                &m1->msg.s1ap_HandoverRequestIEs,
                &m2->msg.s1ap_HandoverRequestIEs);
      } else if (present == S1AP_PDU_PR_successfulOutcome) {
        ret = s1ap_compare_s1ap_handoverrequestacknowledgeies(
                &m1->msg.s1ap_HandoverRequestAcknowledgeIEs,
                &m2->msg.s1ap_HandoverRequestAcknowledgeIEs);
      } else if (present == S1AP_PDU_PR_unsuccessfulOutcome) {
        ret = s1ap_compare_s1ap_handoverfailureies(
                &m1->msg.s1ap_HandoverFailureIEs,
                &m2->msg.s1ap_HandoverFailureIEs);
      }
      break;

    case  S1ap_ProcedureCode_id_HandoverNotification:
        ret = s1ap_compare_s1ap_handovernotifyies(
                &m1->msg.s1ap_HandoverNotifyIEs,
                &m2->msg.s1ap_HandoverNotifyIEs);
      break;

    case  S1ap_ProcedureCode_id_PathSwitchRequest:
      ret = s1ap_compare_s1ap_pathswitchrequesties(
              &m1->msg.s1ap_PathSwitchRequestIEs,
              &m2->msg.s1ap_PathSwitchRequestIEs);
      break;

    case  S1ap_ProcedureCode_id_HandoverCancel:
      ret = s1ap_compare_s1ap_handovercancelies(
              &m1->msg.s1ap_HandoverCancelIEs,
              &m2->msg.s1ap_HandoverCancelIEs);
      break;

    case  S1ap_ProcedureCode_id_E_RABSetup:
      if (present == S1AP_PDU_PR_initiatingMessage) {
        ret = s1ap_compare_s1ap_e_rabsetuprequesties(
                &m1->msg.s1ap_E_RABSetupRequestIEs,
                &m2->msg.s1ap_E_RABSetupRequestIEs);
      } else  {
        ret = s1ap_compare_s1ap_e_rabsetupresponseies(
                &m1->msg.s1ap_E_RABSetupResponseIEs,
                &m2->msg.s1ap_E_RABSetupResponseIEs);
      }
      break;

    case  S1ap_ProcedureCode_id_E_RABModify:
      if (present == S1AP_PDU_PR_initiatingMessage) {
        ret = s1ap_compare_s1ap_e_rabmodifyrequesties(
                &m1->msg.s1ap_E_RABModifyRequestIEs,
                &m2->msg.s1ap_E_RABModifyRequestIEs);
      } else  {
        ret = s1ap_compare_s1ap_e_rabmodifyresponseies(
                &m1->msg.s1ap_E_RABModifyResponseIEs,
                &m2->msg.s1ap_E_RABModifyResponseIEs);
      }
      break;

    case  S1ap_ProcedureCode_id_E_RABRelease:
      if (present == S1AP_PDU_PR_initiatingMessage) {
        ret = s1ap_compare_s1ap_e_rabreleasecommandies(
                &m1->msg.s1ap_E_RABReleaseCommandIEs,
                &m2->msg.s1ap_E_RABReleaseCommandIEs);
      } else  {
        ret = s1ap_compare_s1ap_e_rabreleaseresponseies(
                &m1->msg.s1ap_E_RABReleaseResponseIEs,
                &m2->msg.s1ap_E_RABReleaseResponseIEs);
      }
      break;

    case  S1ap_ProcedureCode_id_E_RABReleaseIndication:
      ret = s1ap_compare_s1ap_e_rabreleaseindicationies(
              &m1->msg.s1ap_E_RABReleaseIndicationIEs,
              &m2->msg.s1ap_E_RABReleaseIndicationIEs);
      break;

    case  S1ap_ProcedureCode_id_InitialContextSetup:
      if (present == S1AP_PDU_PR_initiatingMessage) {
        ret = s1ap_compare_s1ap_initialcontextsetuprequesties(
                &m1->msg.s1ap_InitialContextSetupRequestIEs,
                &m2->msg.s1ap_InitialContextSetupRequestIEs);
      } else  {
        ret = s1ap_compare_s1ap_initialcontextsetupresponseies(
                &m1->msg.s1ap_InitialContextSetupResponseIEs,
                &m2->msg.s1ap_InitialContextSetupResponseIEs);
      }
      break;

    case  S1ap_ProcedureCode_id_Paging:
      ret = s1ap_compare_s1ap_pagingies(
              &m1->msg.s1ap_PagingIEs,
              &m2->msg.s1ap_PagingIEs);
      break;

    case  S1ap_ProcedureCode_id_downlinkNASTransport:
      ret = s1ap_compare_s1ap_downlinknastransporties(
              &m1->msg.s1ap_DownlinkNASTransportIEs,
              &m2->msg.s1ap_DownlinkNASTransportIEs);
      break;

    case  S1ap_ProcedureCode_id_initialUEMessage:
      ret = s1ap_compare_s1ap_initialuemessageies(
              &m1->msg.s1ap_InitialUEMessageIEs,
              &m2->msg.s1ap_InitialUEMessageIEs);
      break;

    case  S1ap_ProcedureCode_id_uplinkNASTransport:
      ret = s1ap_compare_s1ap_uplinknastransporties(
              &m1->msg.s1ap_UplinkNASTransportIEs,
              &m2->msg.s1ap_UplinkNASTransportIEs);
      break;

    case  S1ap_ProcedureCode_id_Reset:
      ret = s1ap_compare_s1ap_reseties(
              &m1->msg.s1ap_ResetIEs,
              &m2->msg.s1ap_ResetIEs);
      break;

    case  S1ap_ProcedureCode_id_ErrorIndication:
      ret = s1ap_compare_s1ap_errorindicationies(
              &m1->msg.s1ap_ErrorIndicationIEs,
              &m2->msg.s1ap_ErrorIndicationIEs);
      break;

    case  S1ap_ProcedureCode_id_NASNonDeliveryIndication:
      ret = s1ap_compare_s1ap_nasnondeliveryindication_ies(
              &m1->msg.s1ap_NASNonDeliveryIndication_IEs,
              &m2->msg.s1ap_NASNonDeliveryIndication_IEs);
      break;

    case  S1ap_ProcedureCode_id_S1Setup:
      if (present == S1AP_PDU_PR_initiatingMessage) {
        ret = s1ap_compare_s1ap_s1setuprequesties(
                &m1->msg.s1ap_S1SetupRequestIEs,
                &m2->msg.s1ap_S1SetupRequestIEs);
      } else if (present == S1AP_PDU_PR_successfulOutcome) {
        ret = s1ap_compare_s1ap_s1setupresponseies(
                &m1->msg.s1ap_S1SetupResponseIEs,
                &m2->msg.s1ap_S1SetupResponseIEs);
      } else if (present == S1AP_PDU_PR_unsuccessfulOutcome) {
        ret = s1ap_compare_s1ap_s1setupfailureies(
                &m1->msg.s1ap_S1SetupFailureIEs,
                &m2->msg.s1ap_S1SetupFailureIEs);
      }
      break;

    case  S1ap_ProcedureCode_id_UEContextReleaseRequest:
      ret = s1ap_compare_s1ap_uecontextreleaserequesties(
                &m1->msg.s1ap_UEContextReleaseRequestIEs,
                &m2->msg.s1ap_UEContextReleaseRequestIEs);
      break;

    case  S1ap_ProcedureCode_id_DownlinkS1cdma2000tunneling:
      ret = s1ap_compare_s1ap_downlinks1cdma2000tunnelingies(
                &m1->msg.s1ap_DownlinkS1cdma2000tunnelingIEs,
                &m2->msg.s1ap_DownlinkS1cdma2000tunnelingIEs);
      break;

    case  S1ap_ProcedureCode_id_UplinkS1cdma2000tunneling:
      ret = s1ap_compare_s1ap_uplinks1cdma2000tunnelingies(
                &m1->msg.s1ap_UplinkS1cdma2000tunnelingIEs,
                &m2->msg.s1ap_UplinkS1cdma2000tunnelingIEs);
      break;

    case  S1ap_ProcedureCode_id_UEContextModification:
      if (present == S1AP_PDU_PR_initiatingMessage) {
        ret = s1ap_compare_s1ap_uecontextmodificationrequesties(
                &m1->msg.s1ap_UEContextModificationRequestIEs,
                &m2->msg.s1ap_UEContextModificationRequestIEs);
      } else if (present == S1AP_PDU_PR_successfulOutcome) {
        ret = s1ap_compare_s1ap_uecontextmodificationresponseies(
                &m1->msg.s1ap_UEContextModificationResponseIEs,
                &m2->msg.s1ap_UEContextModificationResponseIEs);
      } else if (present == S1AP_PDU_PR_unsuccessfulOutcome) {
        ret = s1ap_compare_s1ap_uecontextmodificationfailureies(
                &m1->msg.s1ap_UEContextModificationFailureIEs,
                &m2->msg.s1ap_UEContextModificationFailureIEs);
      }
      break;

    case  S1ap_ProcedureCode_id_UECapabilityInfoIndication:
      ret = s1ap_compare_s1ap_uecapabilityinfoindicationies(
                &m1->msg.s1ap_UECapabilityInfoIndicationIEs,
                &m2->msg.s1ap_UECapabilityInfoIndicationIEs);
      break;

    case  S1ap_ProcedureCode_id_UEContextRelease:
      if (present == S1AP_PDU_PR_initiatingMessage) {
        ret = s1ap_compare_s1ap_uecontextreleasecommandies(
                &m1->msg.s1ap_UEContextReleaseCommandIEs,
                &m2->msg.s1ap_UEContextReleaseCommandIEs);
      } else  {
        ret = s1ap_compare_s1ap_uecontextreleasecompleteies(
                &m1->msg.s1ap_UEContextReleaseCompleteIEs,
                &m2->msg.s1ap_UEContextReleaseCompleteIEs);
      }
      break;

    case  S1ap_ProcedureCode_id_eNBStatusTransfer:
      ret = s1ap_compare_s1ap_enbstatustransferies(
                &m1->msg.s1ap_ENBStatusTransferIEs,
                &m2->msg.s1ap_ENBStatusTransferIEs);
      break;

    case  S1ap_ProcedureCode_id_MMEStatusTransfer:
      ret = s1ap_compare_s1ap_mmestatustransferies(
                &m1->msg.s1ap_MMEStatusTransferIEs,
                &m2->msg.s1ap_MMEStatusTransferIEs);
      break;

    case  S1ap_ProcedureCode_id_DeactivateTrace:
      ret = s1ap_compare_s1ap_deactivatetraceies(
                &m1->msg.s1ap_DeactivateTraceIEs,
                &m2->msg.s1ap_DeactivateTraceIEs);
      break;

    case  S1ap_ProcedureCode_id_TraceStart:
      ret = s1ap_compare_s1ap_tracestarties(
                &m1->msg.s1ap_TraceStartIEs,
                &m2->msg.s1ap_TraceStartIEs);
      break;

    case  S1ap_ProcedureCode_id_TraceFailureIndication:
      ret = s1ap_compare_s1ap_tracefailureindicationies(
                &m1->msg.s1ap_TraceFailureIndicationIEs,
                &m2->msg.s1ap_TraceFailureIndicationIEs);
      break;

    case  S1ap_ProcedureCode_id_ENBConfigurationUpdate:
      if (present == S1AP_PDU_PR_initiatingMessage) {
        ret = s1ap_compare_s1ap_enbconfigurationupdateies(
                &m1->msg.s1ap_ENBConfigurationUpdateIEs,
                &m2->msg.s1ap_ENBConfigurationUpdateIEs);
      } else if (present == S1AP_PDU_PR_successfulOutcome) {
        ret = s1ap_compare_s1ap_enbconfigurationupdateacknowledgeies(
                &m1->msg.s1ap_ENBConfigurationUpdateAcknowledgeIEs,
                &m2->msg.s1ap_ENBConfigurationUpdateAcknowledgeIEs);
      } else {
        ret = s1ap_compare_s1ap_enbconfigurationupdatefailureies(
                &m1->msg.s1ap_ENBConfigurationUpdateFailureIEs,
                &m2->msg.s1ap_ENBConfigurationUpdateFailureIEs);
      }
      break;

    case  S1ap_ProcedureCode_id_MMEConfigurationUpdate:
      if (present == S1AP_PDU_PR_initiatingMessage) {
        ret = s1ap_compare_s1ap_mmeconfigurationupdateies(
                &m1->msg.s1ap_MMEConfigurationUpdateIEs,
                &m2->msg.s1ap_MMEConfigurationUpdateIEs);
      } else if (present == S1AP_PDU_PR_successfulOutcome) {
        ret = s1ap_compare_s1ap_mmeconfigurationupdateacknowledgeies(
                &m1->msg.s1ap_MMEConfigurationUpdateAcknowledgeIEs,
                &m2->msg.s1ap_MMEConfigurationUpdateAcknowledgeIEs);
      } else {
        ret = s1ap_compare_s1ap_mmeconfigurationupdatefailureies(
                &m1->msg.s1ap_MMEConfigurationUpdateFailureIEs,
                &m2->msg.s1ap_MMEConfigurationUpdateFailureIEs);
      }
      break;

    case  S1ap_ProcedureCode_id_LocationReportingControl:
      ret = s1ap_compare_s1ap_locationreportingcontrolies(
                &m1->msg.s1ap_LocationReportingControlIEs,
                &m2->msg.s1ap_LocationReportingControlIEs);
      break;

    case  S1ap_ProcedureCode_id_LocationReportingFailureIndication:
      ret = s1ap_compare_s1ap_locationreportingfailureindicationies(
                &m1->msg.s1ap_LocationReportingFailureIndicationIEs,
                &m2->msg.s1ap_LocationReportingFailureIndicationIEs);
      break;

    case  S1ap_ProcedureCode_id_LocationReport:
      ret = s1ap_compare_s1ap_locationreporties(
                &m1->msg.s1ap_LocationReportIEs,
                &m2->msg.s1ap_LocationReportIEs);
      break;

    case  S1ap_ProcedureCode_id_OverloadStart:
      ret = s1ap_compare_s1ap_overloadstarties(
                &m1->msg.s1ap_OverloadStartIEs,
                &m2->msg.s1ap_OverloadStartIEs);
      break;

    case  S1ap_ProcedureCode_id_OverloadStop:
      ret = s1ap_compare_s1ap_overloadstopies(
                &m1->msg.s1ap_OverloadStopIEs,
                &m2->msg.s1ap_OverloadStopIEs);
      break;

    case  S1ap_ProcedureCode_id_WriteReplaceWarning:
      if (present == S1AP_PDU_PR_initiatingMessage) {
        ret = s1ap_compare_s1ap_writereplacewarningrequesties(
                &m1->msg.s1ap_WriteReplaceWarningRequestIEs,
                &m2->msg.s1ap_WriteReplaceWarningRequestIEs);
      } else  {
        ret = s1ap_compare_s1ap_writereplacewarningresponseies(
                &m1->msg.s1ap_WriteReplaceWarningResponseIEs,
                &m2->msg.s1ap_WriteReplaceWarningResponseIEs);
      }
      break;

    case  S1ap_ProcedureCode_id_eNBDirectInformationTransfer:
      ret = s1ap_compare_s1ap_enbdirectinformationtransferies(
                &m1->msg.s1ap_ENBDirectInformationTransferIEs,
                &m2->msg.s1ap_ENBDirectInformationTransferIEs);
      break;

    case  S1ap_ProcedureCode_id_MMEDirectInformationTransfer:

      ret = s1ap_compare_s1ap_mmedirectinformationtransferies(
                &m1->msg.s1ap_MMEDirectInformationTransferIEs,
                &m2->msg.s1ap_MMEDirectInformationTransferIEs);
      break;

    case  S1ap_ProcedureCode_id_PrivateMessage:
    case  S1ap_ProcedureCode_id_eNBConfigurationTransfer:
    case  S1ap_ProcedureCode_id_MMEConfigurationTransfer:
      AssertFatal(0, "TODO");
      break;

    case  S1ap_ProcedureCode_id_CellTrafficTrace:
      ret = s1ap_compare_s1ap_celltraffictraceies(
                &m1->msg.s1ap_CellTrafficTraceIEs,
                &m2->msg.s1ap_CellTrafficTraceIEs);
      break;

    case  S1ap_ProcedureCode_id_Kill:
      if (present == S1AP_PDU_PR_initiatingMessage) {
        ret = s1ap_compare_s1ap_killrequesties(
                &m1->msg.s1ap_KillRequestIEs,
                &m2->msg.s1ap_KillRequestIEs);
      } else  {
        ret = s1ap_compare_s1ap_killresponseies(
                &m1->msg.s1ap_KillResponseIEs,
                &m2->msg.s1ap_KillResponseIEs);
      }
      break;

    case  S1ap_ProcedureCode_id_downlinkUEAssociatedLPPaTransport:
      ret = s1ap_compare_s1ap_downlinkueassociatedlppatransport_ies(
                &m1->msg.s1ap_DownlinkUEAssociatedLPPaTransport_IEs,
                &m2->msg.s1ap_DownlinkUEAssociatedLPPaTransport_IEs);
      break;

    case  S1ap_ProcedureCode_id_uplinkUEAssociatedLPPaTransport:
      ret = s1ap_compare_s1ap_uplinkueassociatedlppatransport_ies(
                &m1->msg.s1ap_UplinkUEAssociatedLPPaTransport_IEs,
                &m2->msg.s1ap_UplinkUEAssociatedLPPaTransport_IEs);
      break;

    case  S1ap_ProcedureCode_id_downlinkNonUEAssociatedLPPaTransport:
      ret = s1ap_compare_s1ap_downlinknonueassociatedlppatransport_ies(
                &m1->msg.s1ap_DownlinkNonUEAssociatedLPPaTransport_IEs,
                &m2->msg.s1ap_DownlinkNonUEAssociatedLPPaTransport_IEs);
      break;

    case  S1ap_ProcedureCode_id_uplinkNonUEAssociatedLPPaTransport:
      ret = s1ap_compare_s1ap_uplinknonueassociatedlppatransport_ies(
                &m1->msg.s1ap_UplinkNonUEAssociatedLPPaTransport_IEs,
                &m2->msg.s1ap_UplinkNonUEAssociatedLPPaTransport_IEs);
      break;

    default:
      AssertFatal(0, "Unknown procedure code %ld", m1->procedureCode);
  }
  return ret;
}


void update_xpath_node_mme_ue_s1ap_id(et_s1ap_t * const s1ap, xmlNode *node, const S1ap_MME_UE_S1AP_ID_t new_id)
{
  xmlNode       *cur_node = NULL;
  xmlAttrPtr     attr     = NULL;
  xmlChar       *xml_char = NULL;
  int            size     = 0;
  int            pos      = 0;
  int            go_deeper_in_tree = 1;
  //S1AP_INFO("%s() mme_ue_s1ap_id %u\n", __FUNCTION__, new_id);

  // modify
  for (cur_node = (xmlNode *)node; cur_node; cur_node = cur_node->next) {
    go_deeper_in_tree = 1;
    if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"field"))) {
      // do not get hidden fields
      xml_char = xmlGetProp((xmlNode *)cur_node, (const xmlChar *)"hide");
      if (NULL != xml_char) {
        if ((!xmlStrcmp(xml_char, (const xmlChar *)"yes"))) {
          go_deeper_in_tree = 0;
        }
        xmlFree(xml_char);
      }
      if (0 < go_deeper_in_tree) {
        // first get size
        xml_char = xmlGetProp((xmlNode *)cur_node, (const xmlChar *)"pos");
        if (NULL != xml_char) {
          pos = strtoul((const char *)xml_char, NULL, 0);
          pos -= s1ap->xml_stream_pos_offset;
          AssertFatal(pos >= 0, "Bad pos %d xml_stream_pos_offset %d", pos, s1ap->xml_stream_pos_offset);
          xmlFree(xml_char);
          xml_char = xmlGetProp((xmlNode *)cur_node, (const xmlChar *)"size");
          if (NULL != xml_char) {
            const xmlChar value_d[32];
            const xmlChar value_h[20];
            const xmlChar showname[64];
            int           ret     = 0;
            int           pos2    = 0;
            unsigned long int uli = 0;
            char          hex[3]  = {0,0,0};
            char         *end_ptr = NULL;

            size = strtoul((const char *)xml_char, NULL, 0);
            xmlFree(xml_char);
            // second: try to set value (always hex)
            ret = snprintf((char *)value_d, 32, "%ld", new_id);
            AssertFatal((ret > 0) && (ret < 32), "Could not convert int to dec str");
            ret = snprintf((char *)value_h, 20, "C0%08X", new_id);
            AssertFatal((ret > 0) && (ret < 20), "Could not convert int to hex str");
            ret = snprintf((char *)showname, 64, "MME-UE-S1AP-ID: %d", new_id);
            AssertFatal((ret > 0) && (ret < 64), "Could not convert int to dec str");

            attr = xmlSetProp((xmlNode *)cur_node, (const xmlChar *)"value", value_h);
            attr = xmlSetProp((xmlNode *)cur_node, (const xmlChar *)"show", value_d);
            attr = xmlSetProp((xmlNode *)cur_node, (const xmlChar *)"showname", showname);
            //TODO update s1ap->binary_stream @pos with new_id_hex, size
            AssertFatal((pos+size) < s1ap->binary_stream_allocated_size, "Rewrite of mme_ue_s1ap_id out of bounds of binary_stream");
            //avoid endianess issues
            do {
              hex[0] = value_h[pos2++];
              hex[1] = value_h[pos2++];
              hex[2] = '\0';
              end_ptr = hex;
              uli = strtoul(hex, &end_ptr, 16);
              AssertFatal((uli != ULONG_MAX) && (end_ptr != NULL) && (*end_ptr == '\0'), "Conversion of hexstring %s failed returned %ld errno %d", hex, uli, errno);
              s1ap->binary_stream[pos++] = (unsigned char)uli;
            } while (pos2 < (2*5));
            // update ASN1
            et_decode_s1ap(s1ap);
            //S1AP_INFO("Updated ASN1 for %s\n", showname);
          }
        }
      }
    }
    if (0 < go_deeper_in_tree) {
      update_xpath_node_mme_ue_s1ap_id(s1ap, cur_node->children, new_id);
    }
  }
}

/**
 * update_xpath_nodes:
 * @nodes:    the nodes set.
 * @value:    the new value for the node(s)
 *
 * Prints the @nodes content to @output.
 * From http://www.xmlsoft.org/examples/#xpath2.c
 */
void update_xpath_nodes_mme_ue_s1ap_id(et_s1ap_t * const s1ap_payload, xmlNodeSetPtr nodes, const S1ap_MME_UE_S1AP_ID_t new_id)
{
  int           size = 0;
  int           i    = 0;
  xmlNode      *s1ap_node = NULL;

  size = (nodes) ? nodes->nodeNr : 0;
  //S1AP_DEBUG("%s() num nodes %u\n", __FUNCTION__, size);

  /*
   * NOTE: the nodes are processed in reverse order, i.e. reverse document
   *       order because xmlNodeSetContent can actually free up descendant
   *       of the node and such nodes may have been selected too ! Handling
   *       in reverse order ensure that descendant are accessed first, before
   *       they get removed. Mixing XPath and modifications on a tree must be
   *       done carefully !
   */
  for(i = size - 1; i >= 0; i--) {
    s1ap_node = nodes->nodeTab[i];
    AssertFatal(NULL != s1ap_node, "One element of resultset of XPATH expression is NULL\n");
    update_xpath_node_mme_ue_s1ap_id(s1ap_payload, s1ap_node, new_id);
    /*
     * All the elements returned by an XPath query are pointers to
     * elements from the tree *except* namespace nodes where the XPath
     * semantic is different from the implementation in libxml2 tree.
     * As a result when a returned node set is freed when
     * xmlXPathFreeObject() is called, that routine must check the
     * element type. But node from the returned set may have been removed
     * by xmlNodeSetContent() resulting in access to freed data.
     * This can be exercised by running valgrind
     * There is 2 ways around it:
     *   - make a copy of the pointers to the nodes from the result set
     *     then call xmlXPathFreeObject() and then modify the nodes
     * or
     *   - remove the reference to the modified nodes from the node set
     *     as they are processed, if they are not namespace nodes.
     */
    if (nodes->nodeTab[i]->type != XML_NAMESPACE_DECL) {
      nodes->nodeTab[i] = NULL;
    }
  }
}
//------------------------------------------------------------------------------
int et_s1ap_update_mme_ue_s1ap_id(et_packet_t * const packet, const S1ap_MME_UE_S1AP_ID_t old_id, const S1ap_MME_UE_S1AP_ID_t new_id)
{


  xmlChar              xpath_expression[ET_XPATH_EXPRESSION_MAX_LENGTH];
  int                  ret       = 0;
  xmlDocPtr            doc       = NULL;
  xmlXPathContextPtr   xpath_ctx = NULL;
  xmlXPathObjectPtr    xpath_obj = NULL;

  //S1AP_DEBUG("%s() packet num %u original frame number %u, mme_ue_s1ap_id %u -> %u\n", __FUNCTION__, packet->packet_number, packet->original_frame_number, old_id, new_id);

  ret = snprintf(xpath_expression, ET_XPATH_EXPRESSION_MAX_LENGTH, "//field[@name=\"s1ap.MME_UE_S1AP_ID\"][@show=\"%u\"]", old_id);
  AssertFatal((ret > 0) && (ret < ET_XPATH_EXPRESSION_MAX_LENGTH), "Could not build XPATH expression err=%d", ret);

  doc = packet->sctp_hdr.u.data_hdr.payload.doc;
  // Create xpath evaluation context
  xpath_ctx = xmlXPathNewContext(doc);
  if(xpath_ctx == NULL) {
      fprintf(stderr,"Error: unable to create new XPath context\n");
      xmlFreeDoc(doc);
      return(-1);
  }

  // Evaluate xpath expression
  xpath_obj = xmlXPathEvalExpression(xpath_expression, xpath_ctx);
  xmlXPathFreeContext(xpath_ctx);
  AssertFatal(xpath_obj != NULL, "Unable to evaluate XPATH expression \"%s\"\n", xpath_expression);

  if(xmlXPathNodeSetIsEmpty(xpath_obj->nodesetval)){
    xmlXPathFreeObject(xpath_obj);
    S1AP_DEBUG("%s() No match \"%s\"packet num %u original frame number %u, mme_ue_s1ap_id %u -> %u\n",
        __FUNCTION__, xpath_expression, packet->packet_number, packet->original_frame_number, old_id, new_id);
    return -1;
  }
  // update selected nodes
  update_xpath_nodes_mme_ue_s1ap_id(&packet->sctp_hdr.u.data_hdr.payload, xpath_obj->nodesetval, new_id);

  // Cleanup of XPath data
  xmlXPathFreeObject(xpath_obj);

  return 0;
}
