/*
 * TradeLookupDB.cpp
 *
 * Copyright (C) 2006-2007 Rilson Nascimento
 *
 * 08 July 2006
 */

#include <transactions.h>

using namespace TPCE;

// Constructor
CTradeLookupDB::CTradeLookupDB(CDBConnection *pDBConn)
: CTxnBaseDB(pDBConn)
{
}

// Destructor
CTradeLookupDB::~CTradeLookupDB()
{
}

// Call Trade Lookup Frame 1
void CTradeLookupDB::DoTradeLookupFrame1(const TTradeLookupFrame1Input *pIn,
		TTradeLookupFrame1Output *pOut)
{
#ifdef DEBUG
	cout << "TLF1" << endl;
#endif

	enum tlf1 {
			i_bid_price=0, i_cash_transaction_amount,
			i_cash_transaction_dts, i_cash_transaction_name, i_exec_name,
			i_is_cash, i_is_market, i_num_found, i_settlement_amount,
			i_settlement_cash_due_date, i_settlement_cash_type,
			i_status, i_trade_history_dts, i_trade_history_status_id,
			i_trade_price
	};

	ostringstream osTrades;
	int i = 0;
	osTrades << pIn->trade_id[i];
	for ( i = 1; i < pIn->max_trades; i++)
	{
		osTrades << "," << pIn->trade_id[i];
	}

	ostringstream osCall;
	osCall << "SELECT * FROM TradeLookupFrame1(" <<
			pIn->max_trades << ",'{" <<
			osTrades.str() << "}')";

	BeginTxn();
	// Isolation level required by Clause 7.4.1.3
	m_Txn->exec("SET TRANSACTION ISOLATION LEVEL READ COMMITTED;");
	result R( m_Txn->exec( osCall.str() ) );
	CommitTxn();

	if (R.empty()) 
	{
		//throw logic_error("TradeLookupFrame1: empty result set");
		cout << "warning: empty result set at DoTradeLookupFrame1" << endl;
		pOut->status = 1;
		return;
	}
	result::const_iterator c = R.begin();

	vector<string> vAux;
	vector<string>::iterator p;

	Tokenize(c[i_bid_price].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		pOut->trade_info[i].bid_price = atof((*p).c_str());
		++i;
	}
	vAux.clear();

	Tokenize(c[i_cash_transaction_amount].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		pOut->trade_info[i].cash_transaction_amount = atof((*p).c_str());
		++i;
	}
	vAux.clear();

	Tokenize(c[i_cash_transaction_dts].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		sscanf((*p).c_str(), "%d-%d-%d %d:%d:%d",
				&pOut->trade_info[i].cash_transaction_dts.year,
				&pOut->trade_info[i].cash_transaction_dts.month,
				&pOut->trade_info[i].cash_transaction_dts.day,
				&pOut->trade_info[i].cash_transaction_dts.hour,
				&pOut->trade_info[i].cash_transaction_dts.minute,
				&pOut->trade_info[i].cash_transaction_dts.second);
		++i;
	}
	vAux.clear();

	Tokenize(c[i_cash_transaction_name].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		strcpy(pOut->trade_info[i].cash_transaction_name, (*p).c_str());
		++i;
	}
	vAux.clear();

	Tokenize(c[i_exec_name].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		strcpy(pOut->trade_info[i].exec_name, (*p).c_str());
		++i;
	}
	vAux.clear();

	Tokenize(c[i_is_cash].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		pOut->trade_info[i].is_cash = atoi((*p).c_str());
		++i;
	}
	vAux.clear();

	Tokenize(c[i_is_market].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		pOut->trade_info[i].is_market = atoi((*p).c_str());
		++i;
	}
	vAux.clear();

	Tokenize(c[i_settlement_amount].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		pOut->trade_info[i].settlement_amount = atof((*p).c_str());
		++i;
	}
	vAux.clear();

	Tokenize(c[i_settlement_cash_due_date].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		sscanf((*p).c_str(), "%d-%d-%d %d:%d:%d",
				&pOut->trade_info[i].settlement_cash_due_date.year,
				&pOut->trade_info[i].settlement_cash_due_date.month,
				&pOut->trade_info[i].settlement_cash_due_date.day,
				&pOut->trade_info[i].settlement_cash_due_date.hour,
				&pOut->trade_info[i].settlement_cash_due_date.minute,
				&pOut->trade_info[i].settlement_cash_due_date.second);
		++i;
	}
	vAux.clear();

	Tokenize(c[i_settlement_cash_type].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		strcpy(pOut->trade_info[i].settlement_cash_type, (*p).c_str());
		++i;
	}
	vAux.clear();

	// FIXME: There are supposed to be up to 3 in an array.
/*
	Tokenize(c[i_trade_history_dts].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		sscanf((*p).c_str(), "%d-%d-%d %d:%d:%d",
				&pOut->trade_info[i].trade_history_dts.year,
				&pOut->trade_info[i].trade_history_dts.month,
				&pOut->trade_info[i].trade_history_dts.day,
				&pOut->trade_info[i].trade_history_dts.hour,
				&pOut->trade_info[i].trade_history_dts.minute,
				&pOut->trade_info[i].trade_history_dts.second);
		++i;
	}
	vAux.clear();

	Tokenize(c[i_trade_history_status_id].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		strcpy(pOut->trade_info[i].trade_history_status_id, (*p).c_str());
		++i;
	}
	vAux.clear();
*/

	Tokenize(c[i_trade_price].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		pOut->trade_info[i].trade_price = atof((*p).c_str());
		++i;
	}
	vAux.clear();

 	pOut->num_found = c[i_num_found].as(int());
	pOut->status = c[i_status].as(int());

#ifdef DEBUG
	m_coutLock.ClaimLock();
	cout<<"Trade Lookup Frame 1 (input)"<<endl
	    <<"- max_trades: "<<pIn->max_trades<<endl
	    <<"- Trades: {"<<osTrades.str()<<"}"<<endl;
	cout << "Trade Lookup Frame 1 (output)"<< endl <<
			"- num_found: " << pOut->num_found << endl <<
			"- bid_price[0]: " << pOut->trade_info[0].bid_price <<
			endl <<
			"- exec_name[0]: " << pOut->trade_info[0].exec_name <<
			endl <<
			"- is_cash[0]: " << pOut->trade_info[0].is_cash << endl <<
			"- is_market[0]: " << pOut->trade_info[0].is_market <<
			endl <<
			"- trade_price[0]: " << pOut->trade_info[0].trade_price <<
			endl <<
			"- settlement_amount[0]: " <<
			pOut->trade_info[0].settlement_amount << endl <<
			"- settlement_cash_due_date[0]: " <<
			pOut->trade_info[0].settlement_cash_due_date.year << "-" <<
			pOut->trade_info[0].settlement_cash_due_date.month <<
			"-" << pOut->trade_info[0].settlement_cash_due_date.day <<
			" " << pOut->trade_info[0].settlement_cash_due_date.hour <<
			":" <<
			pOut->trade_info[0].settlement_cash_due_date.minute <<
			":" <<
			pOut->trade_info[0].settlement_cash_due_date.second <<
			endl <<
			"- settlement_cash_type[0]: " <<
			pOut->trade_info[0].settlement_cash_type << endl <<
			"- cash_transaction_amount[0]: " <<
			pOut->trade_info[0].cash_transaction_amount << endl <<
			"- cash_transaction_dts[0]: " <<
			pOut->trade_info[0].cash_transaction_dts.year << "-" <<
			pOut->trade_info[0].cash_transaction_dts.month << "-" <<
			pOut->trade_info[0].cash_transaction_dts.day << " "<<
			pOut->trade_info[0].cash_transaction_dts.hour << ":" <<
			pOut->trade_info[0].cash_transaction_dts.minute << ":" <<
			pOut->trade_info[0].cash_transaction_dts.second << endl <<
			"- cash_transaction_name[0]: " <<
			pOut->trade_info[0].cash_transaction_name << endl <<
			"- trade_history_dts[0][0]: " <<
			pOut->trade_info[0].trade_history_dts[0].year << "-" <<
			pOut->trade_info[0].trade_history_dts[0].month << "-" <<
			pOut->trade_info[0].trade_history_dts[0].day << " " <<
			pOut->trade_info[0].trade_history_dts[0].hour << ":" <<
			pOut->trade_info[0].trade_history_dts[0].minute << ":" <<
			pOut->trade_info[0].trade_history_dts[0].second << endl <<
			"- trade_history_status_id[0][0]: " <<
			pOut->trade_info[0].trade_history_status_id[0] << endl <<
			"- trade_history_dts[0][1]: " <<
			pOut->trade_info[0].trade_history_dts[1].year << "-" <<
			pOut->trade_info[0].trade_history_dts[1].month << "-" <<
			pOut->trade_info[0].trade_history_dts[1].day << " " <<
			pOut->trade_info[0].trade_history_dts[1].hour << ":" <<
			pOut->trade_info[0].trade_history_dts[1].minute << ":" <<
			pOut->trade_info[0].trade_history_dts[1].second << endl <<
			"- trade_history_status_id[0][1]: " <<
			pOut->trade_info[0].trade_history_status_id[1] << endl <<
			"- trade_history_dts[0][2]: " <<
			pOut->trade_info[0].trade_history_dts[2].year << "-" <<
			pOut->trade_info[0].trade_history_dts[2].month << "-" <<
			pOut->trade_info[0].trade_history_dts[2].day << " " <<
			pOut->trade_info[0].trade_history_dts[2].hour << ":" <<
			pOut->trade_info[0].trade_history_dts[2].minute << ":" <<
			pOut->trade_info[0].trade_history_dts[2].second << endl <<
			"- trade_history_status_id[0][2]: " <<
			pOut->trade_info[0].trade_history_status_id[2] << endl;
	m_coutLock.ReleaseLock();
#endif // DEBUG
}

// Call Trade Lookup Frame 2
void CTradeLookupDB::DoTradeLookupFrame2(const TTradeLookupFrame2Input *pIn,
		TTradeLookupFrame2Output *pOut)
{
#ifdef DEBUG
	cout << "TLF2" << endl;
#endif

	enum tlf2 {
			i_bid_price=0, i_cash_transaction_amount, i_cash_transaction_dts,
			i_cash_transaction_name, i_exec_name, i_is_cash, i_num_found,
			i_settlement_amount, i_settlement_cash_due_date,
			i_settlement_cash_type, i_status, i_trade_history_dts,
			i_trade_history_status_id, i_trade_list, i_trade_price
	};

	ostringstream osCall;
	osCall << "SELECT * FROM TradeLookupFrame2(" <<
			pIn->acct_id << ",'" <<
			pIn->end_trade_dts.year << "-" <<
			pIn->end_trade_dts.month << "-" <<
			pIn->end_trade_dts.day << " " <<
			pIn->end_trade_dts.hour << ":" <<
			pIn->end_trade_dts.minute << ":" <<
			pIn->end_trade_dts.second << "'," <<
			pIn->max_trades << ",'" <<
			pIn->start_trade_dts.year << "-" <<
			pIn->start_trade_dts.month << "-" <<
			pIn->start_trade_dts.day << " " <<
			pIn->start_trade_dts.hour << ":" <<
			pIn->start_trade_dts.minute << ":" <<
			pIn->start_trade_dts.second << "')";

	BeginTxn();
	// Isolation level required by Clause 7.4.1.3
	m_Txn->exec("SET TRANSACTION ISOLATION LEVEL READ COMMITTED;");
	result R( m_Txn->exec( osCall.str() ) );
	CommitTxn();

	if (R.empty()) 
	{
		//throw logic_error("TradeLookupFrame2: empty result set");
		cout<<"warning: empty result set at DoTradeLookupFrame2"<<endl;
		pOut->status = 1;
		return;
	}
	result::const_iterator c = R.begin();

	vector<string> vAux;
	vector<string>::iterator p;

	Tokenize(c[i_bid_price].c_str(), vAux);
	int i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		pOut->trade_info[i].bid_price = atof((*p).c_str());
		++i;
	}
	vAux.clear();

	Tokenize(c[i_cash_transaction_amount].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		pOut->trade_info[i].cash_transaction_amount = atof((*p).c_str());
		++i;
	}
	vAux.clear();

	Tokenize(c[i_cash_transaction_dts].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		sscanf((*p).c_str(), "%d-%d-%d %d:%d:%d",
				&pOut->trade_info[i].cash_transaction_dts.year,
				&pOut->trade_info[i].cash_transaction_dts.month,
				&pOut->trade_info[i].cash_transaction_dts.day,
				&pOut->trade_info[i].cash_transaction_dts.hour,
				&pOut->trade_info[i].cash_transaction_dts.minute,
				&pOut->trade_info[i].cash_transaction_dts.second);
		++i;
	}
	vAux.clear();

	Tokenize(c[i_cash_transaction_name].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		strcpy(pOut->trade_info[i].cash_transaction_name, (*p).c_str());
		++i;
	}
	vAux.clear();

	Tokenize(c[i_exec_name].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		strcpy(pOut->trade_info[i].exec_name, (*p).c_str());
		++i;
	}
	vAux.clear();

	Tokenize(c[i_is_cash].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		pOut->trade_info[i].is_cash = atoi((*p).c_str());
		++i;
	}
	vAux.clear();

	Tokenize(c[i_settlement_amount].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		pOut->trade_info[i].settlement_amount = atof((*p).c_str());
		++i;
	}
	vAux.clear();

	Tokenize(c[i_settlement_cash_due_date].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		sscanf((*p).c_str(), "%d-%d-%d %d:%d:%d",
				&pOut->trade_info[i].settlement_cash_due_date.year,
				&pOut->trade_info[i].settlement_cash_due_date.month,
				&pOut->trade_info[i].settlement_cash_due_date.day,
				&pOut->trade_info[i].settlement_cash_due_date.hour,
				&pOut->trade_info[i].settlement_cash_due_date.minute,
				&pOut->trade_info[i].settlement_cash_due_date.second);
		++i;
	}
	vAux.clear();

	Tokenize(c[i_settlement_cash_type].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		strcpy(pOut->trade_info[i].settlement_cash_type, (*p).c_str());
		++i;
	}
	vAux.clear();

	// FIXME: Add code to handle the trade_history_dts and
	// trade_history_status_id multi-dimensional array data.

	Tokenize(c[i_trade_price].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		pOut->trade_info[i].trade_price = atof((*p).c_str());
		++i;
	}
	vAux.clear();

 	pOut->num_found = c[i_num_found].as(int());
	pOut->status = c[i_status].as(int());

#ifdef DEBUG
	m_coutLock.ClaimLock();
	cout<<"Trade Lookup Frame 2 (input)"<<endl
	    <<"- acct_id: "<<pIn->acct_id<<endl
	    <<"- max_trades: "<<pIn->max_trades<<endl
	    <<"- trade_dts: "<<pIn->end_trade_dts.year<<"-"<<
		pIn->end_trade_dts.month<<"-"<<
		pIn->end_trade_dts.day<<" "<<
		pIn->end_trade_dts.hour<<":"<<
		pIn->end_trade_dts.minute<<":"<<
		pIn->end_trade_dts.second<<endl;
	cout << "Trade Lookup Frame 2 (output)" << endl <<
			"- num_found: " << pOut->num_found << endl <<
			"- bid_price[0]: " << pOut->trade_info[0].bid_price <<
			endl <<
			"- exec_name[0]: " << pOut->trade_info[0].exec_name <<
			endl <<
			"- is_cash[0]: " << pOut->trade_info[0].is_cash << endl <<
			"- trade_price[0]: " << pOut->trade_info[0].trade_price <<
			endl <<
			"- trade_id[0]: " << pOut->trade_info[0].trade_id <<
			endl <<
			"- settlement_amount[0]: " <<
			pOut->trade_info[0].settlement_amount << endl <<
			"- settlement_cash_due_date[0]: " <<
			pOut->trade_info[0].settlement_cash_due_date.year << "-"<<
			pOut->trade_info[0].settlement_cash_due_date.month <<
			"-" << pOut->trade_info[0].settlement_cash_due_date.day <<
			" " << pOut->trade_info[0].settlement_cash_due_date.hour <<
			":" <<
			pOut->trade_info[0].settlement_cash_due_date.minute <<
			":" <<
			pOut->trade_info[0].settlement_cash_due_date.second <<
			endl << "- settlement_cash_type[0]: " <<
			pOut->trade_info[0].settlement_cash_type << endl <<
			"- cash_transaction_amount[0]: " <<
			pOut->trade_info[0].cash_transaction_amount << endl <<
			"- cash_transaction_dts[0]: " <<
			pOut->trade_info[0].cash_transaction_dts.year << "-" <<
			pOut->trade_info[0].cash_transaction_dts.month << "-" <<
			pOut->trade_info[0].cash_transaction_dts.day << " " <<
			pOut->trade_info[0].cash_transaction_dts.hour << ":" <<
			pOut->trade_info[0].cash_transaction_dts.minute << ":" <<
			pOut->trade_info[0].cash_transaction_dts.second << endl <<
			"- cash_transaction_name[0]: " <<
			pOut->trade_info[0].cash_transaction_name << endl <<
			"- trade_history_dts[0][0]: " <<
			pOut->trade_info[0].trade_history_dts[0].year << "-" <<
			pOut->trade_info[0].trade_history_dts[0].month << "-" <<
			pOut->trade_info[0].trade_history_dts[0].day << " " <<
			pOut->trade_info[0].trade_history_dts[0].hour << ":"<<
			pOut->trade_info[0].trade_history_dts[0].minute << ":" <<
			pOut->trade_info[0].trade_history_dts[0].second << endl <<
			"- trade_history_status_id[0][0]: " <<
			pOut->trade_info[0].trade_history_status_id[0] << endl <<
			"- trade_history_dts[0][1]: " <<
			pOut->trade_info[0].trade_history_dts[1].year << "-" <<
			pOut->trade_info[0].trade_history_dts[1].month << "-" <<
			pOut->trade_info[0].trade_history_dts[1].day << " " <<
			pOut->trade_info[0].trade_history_dts[1].hour << ":" <<
			pOut->trade_info[0].trade_history_dts[1].minute << ":" <<
			pOut->trade_info[0].trade_history_dts[1].second << endl <<
			"- trade_history_status_id[0][1]: " <<
			pOut->trade_info[0].trade_history_status_id[1] << endl <<
			"- trade_history_dts[0][2]: " <<
			pOut->trade_info[0].trade_history_dts[2].year << "-" <<
			pOut->trade_info[0].trade_history_dts[2].month << "-" <<
			pOut->trade_info[0].trade_history_dts[2].day << " " <<
			pOut->trade_info[0].trade_history_dts[2].hour << ":" <<
			pOut->trade_info[0].trade_history_dts[2].minute << ":" <<
			pOut->trade_info[0].trade_history_dts[2].second << endl <<
			"- trade_history_status_id[0][2]: " <<
			pOut->trade_info[0].trade_history_status_id[2] << endl;
	m_coutLock.ReleaseLock();
#endif // DEBUG
}

// Call Trade Lookup Frame 3
void CTradeLookupDB::DoTradeLookupFrame3(const TTradeLookupFrame3Input *pIn,
		TTradeLookupFrame3Output *pOut)
{
#ifdef DEBUG
	cout << "TLF3" << endl;
#endif

	enum tlf3 {
            i_acct_id=0, i_cash_transaction_amount, i_cash_transaction_dts,
            i_cash_transaction_name, i_exec_name, i_is_cash, i_num_found,
            i_price, i_quantity, i_settlement_amount,
            i_settlement_cash_due_date, i_settlement_cash_type, i_status,
            i_trade_dts, i_trade_history_dts, i_trade_list, i_trade_type
	};

	ostringstream osCall;
	osCall << "SELECT * FROM TradeLookupFrame3('" <<
			pIn->end_trade_dts.year << "-" <<
			pIn->end_trade_dts.month << "-" <<
			pIn->end_trade_dts.day << " " <<
			pIn->end_trade_dts.hour << ":" <<
			pIn->end_trade_dts.minute << ":" <<
			pIn->end_trade_dts.second << "'," <<
			pIn->max_acct_id << "," <<
			pIn->max_trades << ",'" <<
			pIn->start_trade_dts.year << "-" <<
			pIn->start_trade_dts.month << "-" <<
			pIn->start_trade_dts.day << " " <<
			pIn->start_trade_dts.hour << ":" <<
			pIn->start_trade_dts.minute << ":" <<
			pIn->start_trade_dts.second << "','" <<
			pIn->symbol << "')";

	BeginTxn();
	// Isolation level required by Clause 7.4.1.3
	m_Txn->exec("SET TRANSACTION ISOLATION LEVEL READ COMMITTED;");
	result R( m_Txn->exec( osCall.str() ) );
	CommitTxn();

	if (R.empty()) 
	{
		//throw logic_error("TradeLookupFrame3: empty result set");
		cout<<"warning: empty result set at DoTradeLookupFrame3"<<endl;
	}
	result::const_iterator c = R.begin();
	
	vector<string> vAux;
	vector<string>::iterator p;

	Tokenize(c[i_acct_id].c_str(), vAux);
	int i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		pOut->trade_info[i].acct_id = atol((*p).c_str());
		++i;
	}
	vAux.clear();

	Tokenize(c[i_cash_transaction_amount].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		pOut->trade_info[i].cash_transaction_amount = atof((*p).c_str());
		++i;
	}
	vAux.clear();

	Tokenize(c[i_cash_transaction_dts].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		sscanf((*p).c_str(), "%d-%d-%d %d:%d:%d",
				&pOut->trade_info[i].cash_transaction_dts.year,
				&pOut->trade_info[i].cash_transaction_dts.month,
				&pOut->trade_info[i].cash_transaction_dts.day,
				&pOut->trade_info[i].cash_transaction_dts.hour,
				&pOut->trade_info[i].cash_transaction_dts.minute,
				&pOut->trade_info[i].cash_transaction_dts.second);
		++i;
	}
	vAux.clear();

	Tokenize(c[i_cash_transaction_name].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		strcpy(pOut->trade_info[i].cash_transaction_name, (*p).c_str());
		++i;
	}
	vAux.clear();

	Tokenize(c[i_exec_name].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		strcpy(pOut->trade_info[i].exec_name, (*p).c_str());
		++i;
	}
	vAux.clear();

	Tokenize(c[i_is_cash].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		// FIXME: Huh, why would an assignment seg fault where sscanf()
		// doesn't?
		//pOut->trade_info[i].is_cash = atoi((*p).c_str());
		sscanf((*p).c_str(), "%d", &pOut->trade_info[i].is_cash);
		++i;
	}
	vAux.clear();

	Tokenize(c[i_price].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		pOut->trade_info[i].price = atof((*p).c_str());
		++i;
	}
	vAux.clear();

	Tokenize(c[i_quantity].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		pOut->trade_info[i].quantity = atoi((*p).c_str());
		++i;
	}
	vAux.clear();

	Tokenize(c[i_settlement_amount].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		pOut->trade_info[i].settlement_amount = atof((*p).c_str());
		++i;
	}
	vAux.clear();

	Tokenize(c[i_settlement_cash_due_date].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		sscanf((*p).c_str(), "%d-%d-%d %d:%d:%d",
				&pOut->trade_info[i].settlement_cash_due_date.year,
				&pOut->trade_info[i].settlement_cash_due_date.month,
				&pOut->trade_info[i].settlement_cash_due_date.day,
				&pOut->trade_info[i].settlement_cash_due_date.hour,
				&pOut->trade_info[i].settlement_cash_due_date.minute,
				&pOut->trade_info[i].settlement_cash_due_date.second);
		++i;
	}
	vAux.clear();

	Tokenize(c[i_settlement_cash_type].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		strcpy(pOut->trade_info[i].settlement_cash_type, (*p).c_str());
		++i;
	}
	vAux.clear();

	// FIXME: Add code to handle the trade_history_dts and
	// trade_history_status_id multi-dimensional array data.

	// FIXME: No trade_list variable in TTradeLookupFrame3TradeInfo, where
	// does this info go???
/*
	Tokenize(c[i_trade_list].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		pOut->trade_info[i].trade_list = atol((*p).c_str());
		++i;
	}
*/

	Tokenize(c[i_trade_type].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		strcpy(pOut->trade_info[i].trade_type, (*p).c_str());
		++i;
	}
	vAux.clear();

 	pOut->num_found = c[i_num_found].as(int());
	pOut->status = c[i_status].as(int());

#ifdef DEBUG
	m_coutLock.ClaimLock();
	cout<<"Trade Lookup Frame 3 (input)"<<endl
	    <<"- max_acct_id: "<<pIn->max_acct_id<<endl
	    <<"- max_trades: "<<pIn->max_trades<<endl
	    <<"- trade_dts: "<<pIn->end_trade_dts.year<<"-"<<
		pIn->end_trade_dts.month<<"-"<<
		pIn->end_trade_dts.day<<" "<<
		pIn->end_trade_dts.hour<<":"<<
		pIn->end_trade_dts.minute<<":"<<
		pIn->end_trade_dts.second<<endl
	    <<"- symbol: "<<pIn->symbol<<endl;
	cout << "Trade Lookup Frame 3 (output)" << endl <<
			"- num_found: " << pOut->num_found << endl <<
			"- acct_id: " << pOut->trade_info[0].acct_id << endl <<
			"- cash_transaction_amount[0]: " <<
			pOut->trade_info[0].cash_transaction_amount << endl <<
			"- cash_transaction_dts[0]: " <<
			pOut->trade_info[0].cash_transaction_dts.year << "-" <<
			pOut->trade_info[0].cash_transaction_dts.month << "-" <<
			pOut->trade_info[0].cash_transaction_dts.day << " " <<
			pOut->trade_info[0].cash_transaction_dts.hour << ":" <<
			pOut->trade_info[0].cash_transaction_dts.minute << ":" <<
			pOut->trade_info[0].cash_transaction_dts.second << endl <<
			"- cash_transaction_name[0]: " <<
			pOut->trade_info[0].cash_transaction_name << endl <<
			"- exec_name[0]: " << pOut->trade_info[0].exec_name <<
			endl << "- is_cash[0]: " << pOut->trade_info[0].is_cash <<
			endl <<
			"- price[0]: " << pOut->trade_info[0].price << endl <<
			"- quantity[0]: " << pOut->trade_info[0].quantity <<
			endl <<
			"- settlement_amount[0]: " <<
			pOut->trade_info[0].settlement_amount << endl <<
			"- settlement_cash_due_date[0]: " <<
			pOut->trade_info[0].settlement_cash_due_date.year << "-" <<
			pOut->trade_info[0].settlement_cash_due_date.month <<
			"-" << pOut->trade_info[0].settlement_cash_due_date.day <<
			" " << pOut->trade_info[0].settlement_cash_due_date.hour <<
			":" <<
			pOut->trade_info[0].settlement_cash_due_date.minute <<
			":" <<
			pOut->trade_info[0].settlement_cash_due_date.second <<
			endl <<
			"- settlement_cash_type[0]: " <<
			pOut->trade_info[0].settlement_cash_type << endl <<
			"- cash_transaction_dts[0]: " <<
			pOut->trade_info[0].trade_dts.year << "-" <<
			pOut->trade_info[0].trade_dts.month << "-" <<
			pOut->trade_info[0].trade_dts.day << " "<<
			pOut->trade_info[0].trade_dts.hour << ":" <<
			pOut->trade_info[0].trade_dts.minute << ":" <<
			pOut->trade_info[0].trade_dts.second << endl <<
			"- trade_history_dts[0][0]: " <<
			pOut->trade_info[0].trade_history_dts[0].year << "-" <<
			pOut->trade_info[0].trade_history_dts[0].month << "-" <<
			pOut->trade_info[0].trade_history_dts[0].day << " " <<
			pOut->trade_info[0].trade_history_dts[0].hour << ":" <<
			pOut->trade_info[0].trade_history_dts[0].minute << ":" <<
			pOut->trade_info[0].trade_history_dts[0].second << endl <<
			"- trade_history_status_id[0][0]: " <<
			pOut->trade_info[0].trade_history_status_id[0] << endl <<
			"- trade_history_dts[0][1]: " <<
			pOut->trade_info[0].trade_history_dts[1].year << "-" <<
			pOut->trade_info[0].trade_history_dts[1].month << "-" <<
			pOut->trade_info[0].trade_history_dts[1].day << " " <<
			pOut->trade_info[0].trade_history_dts[1].hour << ":" <<
			pOut->trade_info[0].trade_history_dts[1].minute << ":" <<
			pOut->trade_info[0].trade_history_dts[1].second << endl <<
			"- trade_history_status_id[0][1]: " <<
			pOut->trade_info[0].trade_history_status_id[1] << endl <<
			"- trade_history_dts[0][2]: " <<
			pOut->trade_info[0].trade_history_dts[2].year << "-" <<
			pOut->trade_info[0].trade_history_dts[2].month << "-" <<
			pOut->trade_info[0].trade_history_dts[2].day << " " <<
			pOut->trade_info[0].trade_history_dts[2].hour << ":" <<
			pOut->trade_info[0].trade_history_dts[2].minute << ":" <<
			pOut->trade_info[0].trade_history_dts[2].second << endl <<
			"- trade_history_status_id[0][2]: " <<
			pOut->trade_info[0].trade_history_status_id[2] << endl <<
			"- trade_id[0]: " << pOut->trade_info[0].trade_id <<
			endl <<
			"- trade_type[0]: " << pOut->trade_info[0].trade_type <<
			endl;
	m_coutLock.ReleaseLock();
#endif //DEBUG
}

// Call Trade Lookup Frame 4
void CTradeLookupDB::DoTradeLookupFrame4(const TTradeLookupFrame4Input *pIn,
		TTradeLookupFrame4Output *pOut)
{
#ifdef DEBUG
	cout << "TLF4" << endl;
#endif

	enum tlf4 {                                                                 
			i_holding_history_id=0, i_holding_history_trade_id, i_num_found,    
			i_quantity_after, i_quantity_before, i_status, i_trade_id           
	};

	ostringstream osCall;
	osCall << "SELECT * FROM TradeLookupFrame4(" <<
			pIn->acct_id << ",'" <<
			pIn->trade_dts.year << "-" <<
			pIn->trade_dts.month << "-" <<
			pIn->trade_dts.day << " " <<
			pIn->trade_dts.hour << ":" <<
			pIn->trade_dts.minute << ":" <<
			pIn->trade_dts.second << "')";

	BeginTxn();
	// Isolation level required by Clause 7.4.1.3
	m_Txn->exec("SET TRANSACTION ISOLATION LEVEL READ COMMITTED;");
	result R( m_Txn->exec( osCall.str() ) );
	CommitTxn();

	if (R.empty()) 
	{
		//throw logic_error("TradeLookupFrame4: empty result set");
		cout<<"warning: empty result set at DoTradeLookupFrame4"<<endl;
 		pOut->num_found = 0;
		pOut->status = CBaseTxnErr::SUCCESS;
		return;
	}
	result::const_iterator c = R.begin();

	vector<string> vAux;
	vector<string>::iterator p;

	Tokenize(c[i_holding_history_id].c_str(), vAux);
	int i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		pOut->trade_info[i].holding_history_id = atol((*p).c_str());
		++i;
	}
	vAux.clear();

	Tokenize(c[i_holding_history_trade_id].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		pOut->trade_info[i].holding_history_trade_id = atol((*p).c_str());
		++i;
	}
	vAux.clear();

	Tokenize(c[i_quantity_after].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		pOut->trade_info[i].quantity_after = atol((*p).c_str());
		++i;
	}
	vAux.clear();

	Tokenize(c[i_quantity_before].c_str(), vAux);
	i = 0;
	for (p = vAux.begin(); p != vAux.end(); ++p) {
		pOut->trade_info[i].quantity_before = atol((*p).c_str());
		++i;
	}
	vAux.clear();

	pOut->trade_id = c[i_num_found].as(int());
	pOut->trade_id = c[i_status].as(int());
	pOut->trade_id = c[i_trade_id].as(long());

#ifdef DEBUG
	m_coutLock.ClaimLock();
	cout<<"Trade Lookup Frame 4 (input)"<<endl
		<<"- acct_id: "<<pIn->acct_id<<endl
		<<"- trade_dts: "<<pIn->trade_dts.year<<"-"<<
		pIn->trade_dts.month<<"-"<<
		pIn->trade_dts.day<<" "<<
		pIn->trade_dts.hour<<":"<<
		pIn->trade_dts.minute<<":"<<
		pIn->trade_dts.second<<endl;
	cout << "Trade Lookup Frame 4 (output)" << endl <<
			"- holding_history_id[0]: " <<
			pOut->trade_info[0].holding_history_id << endl <<
			"- holding_history_trade_id[0]: " <<
			pOut->trade_info[0].holding_history_trade_id << endl <<
			"- quantity_before[0]: " <<
			pOut->trade_info[0].quantity_before << endl <<
			"- quantity_after[0]: " <<
			pOut->trade_info[0].quantity_after << endl <<
			"- num_found: " << pOut->num_found << endl <<
			"- trade_id: " << pOut->trade_id << endl;
	m_coutLock.ReleaseLock();
#endif //DEBUG
}


// Clean-up
void CTradeLookupDB::Cleanup(void* pException)
{
}
