/*
 * Legal Notice
 *
 * This document and associated source code (the "Work") is a part of a
 * benchmark specification maintained by the TPC.
 *
 * The TPC reserves all right, title, and interest to the Work as provided
 * under U.S. and international laws, including without limitation all patent
 * and trademark rights therein.
 *
 * No Warranty
 *
 * 1.1 TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THE INFORMATION
 *     CONTAINED HEREIN IS PROVIDED "AS IS" AND WITH ALL FAULTS, AND THE
 *     AUTHORS AND DEVELOPERS OF THE WORK HEREBY DISCLAIM ALL OTHER
 *     WARRANTIES AND CONDITIONS, EITHER EXPRESS, IMPLIED OR STATUTORY,
 *     INCLUDING, BUT NOT LIMITED TO, ANY (IF ANY) IMPLIED WARRANTIES,
 *     DUTIES OR CONDITIONS OF MERCHANTABILITY, OF FITNESS FOR A PARTICULAR
 *     PURPOSE, OF ACCURACY OR COMPLETENESS OF RESPONSES, OF RESULTS, OF
 *     WORKMANLIKE EFFORT, OF LACK OF VIRUSES, AND OF LACK OF NEGLIGENCE.
 *     ALSO, THERE IS NO WARRANTY OR CONDITION OF TITLE, QUIET ENJOYMENT,
 *     QUIET POSSESSION, CORRESPONDENCE TO DESCRIPTION OR NON-INFRINGEMENT
 *     WITH REGARD TO THE WORK.
 * 1.2 IN NO EVENT WILL ANY AUTHOR OR DEVELOPER OF THE WORK BE LIABLE TO
 *     ANY OTHER PARTY FOR ANY DAMAGES, INCLUDING BUT NOT LIMITED TO THE
 *     COST OF PROCURING SUBSTITUTE GOODS OR SERVICES, LOST PROFITS, LOSS
 *     OF USE, LOSS OF DATA, OR ANY INCIDENTAL, CONSEQUENTIAL, DIRECT,
 *     INDIRECT, OR SPECIAL DAMAGES WHETHER UNDER CONTRACT, TORT, WARRANTY,
 *     OR OTHERWISE, ARISING IN ANY WAY OUT OF THIS OR ANY OTHER AGREEMENT
 *     RELATING TO THE WORK, WHETHER OR NOT SUCH AUTHOR OR DEVELOPER HAD
 *     ADVANCE NOTICE OF THE POSSIBILITY OF SUCH DAMAGES.
 *
 * Contributors
 * - 2006 Rilson Nascimento
 * - 2010 Mark Wong <markwkm@postgresql.org>
 */

/*
 * Database loader class for COMPANY table.
 */

#ifndef PGSQL_COMPANY_LOAD_H
#define PGSQL_COMPANY_LOAD_H

namespace TPCE
{

class CPGSQLCompanyLoad : public CPGSQLLoader<COMPANY_ROW>
{
private:
	CDateTime co_open_date;

public:
	CPGSQLCompanyLoad(char *szConnectStr, char *szTable = "company")
			: CPGSQLLoader<COMPANY_ROW>(szConnectStr, szTable) { };

	// copy to the bound location inside this class first
	virtual void WriteNextRecord(PT next_record) {
		co_open_date = next_record->CO_OPEN_DATE;

		fprintf(p, "%ld%c%s%c%s%c%s%c%s%c%s%c%ld%c%s%c%s\n",
				next_record->CO_ID, delimiter,
				next_record->CO_ST_ID, delimiter,
				next_record->CO_NAME, delimiter,
				next_record->CO_IN_ID, delimiter,
				next_record->CO_SP_RATE, delimiter,
				next_record->CO_CEO, delimiter,
				next_record->CO_AD_ID, delimiter,
				next_record->CO_DESC, delimiter,
				co_open_date.ToStr(iDateTimeFmt));
		// FIXME: Have blind faith that this row of data was built correctly.
		while (fgetc(p) != EOF) ;
	}
};

} // namespace TPCE

#endif // PGSQL_COMPANY_LOAD_H
