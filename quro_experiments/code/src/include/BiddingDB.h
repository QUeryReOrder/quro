#ifndef BIDDING_DB_H
#define BIDDING_DB_H

#include "DBConnection.h"
#include "TxnBaseDB.h"
#include "CommonStructs.h"

class CBiddingDB: public CTxnBaseDB
{
public:
		CBiddingDB(CDBConnection *pDBConn): CTxnBaseDB(pDBConn) {};
		void DoBidding(TBiddingTxnInput* pIn, TBiddingTxnOutput* pOut);
		void execute(const TBiddingTxnInput* pIn, TBiddingTxnOutput* pOut);
};

#define GET_USER \
"SELECT BID, NAME \n" \
"  FROM USER \n" \
"  WHERE USER_ID = %ld"

#define UPDATE_USER \
"UPDATE USER SET BID = BID + 1 \n" \
"	WHERE USER_ID = %ld"

#define INSERT_BID \
"INSERT INTO BIDREC (" \
" USER_ID, BID, PID, PRICE, DESCRIPTION ) " \
" VALUES (%ld, %ld, %ld, %f, '%s')"

#define SELECT_ITEM \
"SELECT MAX_PRICE, DESCRIPTION \n" \
"  FROM BIDITEM \n" \
"  WHERE PID = %ld"

#define UPDATE_ITEM \
"UPDATE BIDITEM SET \n" \
"  MAX_PRICE = %f \n" \
"  WHERE PID = %d"

#define FAIL_MSG(s) string msg(s); \
	throw msg.c_str();

#define BIDDING_UPDATE_ITEM sprintf(query, UPDATE_ITEM, bid_price, pIn->item_id); \
			if(!dbt5_sql_execute(query, &result, "UPDATE_ITEM")){ \
				FAIL_MSG("UPDATE ITEM FAIL"); \
			} \

#define BIDDING_UPDATE_USER sprintf(query, UPDATE_USER, pIn->user_id); \
	if(!dbt5_sql_execute(query, &result, "UPDATE USER")){ \
		FAIL_MSG("UPDATE USER FAIL"); \
	} \

#define BIDDING_INSERT_REC sprintf(query, INSERT_BID, pIn->user_id, bid+1, pIn->item_id, bid_price, rec_description.c_str()); \
	if(!dbt5_sql_execute(query, &result, "INSERT REC")){ \
		FAIL_MSG("INSERT REC FAIL"); \
	} \


#endif /* BIDDING_DB_H */
