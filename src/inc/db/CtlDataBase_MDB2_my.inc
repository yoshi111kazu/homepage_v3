<?php
/*****************************************************************************
 *  name        : CtlDataBase_MDB2_my.inc
 *
 *  create      : 2008/01/08
 ****************************************************************************/
require_once( "MDB2.php" );

class CtlDataBase {
	var $db;
	var $DATABASE_USER;			// ユーザ
	var $DATABASE_PASSWORD;		// パスワード
	var $DATABASE_HOST;			// ホスト名
	var $DATABASE_NAME;			// データベース名
	var $TABLE_INFO;			// テーブル情報

##**************************************************************************
##  コンストラクタ
##**************************************************************************
	function CtlDataBase( $flDefine ){
		$this->db = null;
		$this->TABLE_INFO = array();

		// 接続情報取得
		require( $flDefine );
		$this->DATABASE_USER = $DB_DSN['DATABASE_USER'];
		$this->DATABASE_PASSWORD = $DB_DSN['DATABASE_PASSWORD'];
		$this->DATABASE_HOST = $DB_DSN['DATABASE_HOST'];
		$this->DATABASE_NAME = $DB_DSN['DATABASE_NAME'];
	}

##**************************************************************************
##  接続関連
##**************************************************************************
	//////////////////////////////////////////////////
	// 接続
	//////////////////////////////////////////////////
	function connectDatabase( $charset=null ){
		// 接続済み
		if( $this->db != null ){
			return $this->db;
		}

		// DSN作成
		$dsn = sprintf( 'mysql://%s:%s@%s/%s%s', $this->DATABASE_USER, $this->DATABASE_PASSWORD, $this->DATABASE_HOST, $this->DATABASE_NAME, $charset );
		// PEARのMDB2モジュールで接続
		$this->db = MDB2::factory( $dsn ,true);

		// エラーチェック
		if( !$this->error_check( $this->db ) ){
			return false;
		}

		return true;
	}

	//////////////////////////////////////////////////
	// 切断
	//////////////////////////////////////////////////
	function disconnectDatabase(){
		// 切断
		$this->db->disconnect();

		// エラーチェック
		if( !$this->error_check( $this->db ) ){
			return false;
		}

		// 初期化
		$this->db = null;

		return true;
	}

##**************************************************************************
##  トランザクション関連
##**************************************************************************
	//////////////////////////////////////////////////
	// BEGIN
	//////////////////////////////////////////////////
	function begin(){
		// トランザクション開始
		$this->db->beginTransaction();

		// BEGIN
		//$result = $this->db->query( "BEGIN" );

		// エラーチェック
		if( !$this->error_check( $result ) ){
			return false;
		}

		return true;
	}

	//////////////////////////////////////////////////
	// コミット
	//////////////////////////////////////////////////
	function commit(){
		// コミット
		$result = $this->db->query( "COMMIT" );

		// エラーチェック
		if( !$this->error_check( $result ) ){
			return false;
		}

		return true;
	}

	//////////////////////////////////////////////////
	// ロールバック
	//////////////////////////////////////////////////
	function rollback(){
		// ロールバック
		$result = $this->db->query( "ROLLBACK" );

		// エラーチェック
		if( !$this->error_check( $result ) ){
			return false;
		}

		return true;
	}

##**************************************************************************
##  クエリ実行関連
##**************************************************************************
	//////////////////////////////////////////////////
	// SELECTを実行
	//////////////////////////////////////////////////
	function doSelect($strSql){
		// SELECT
		$result = $this->db->query($strSql);

		// エラーチェック
		if( !$this->error_check( $result ) ){
			return false;
		}

		// テーブル情報取得
		$this->doTableInfo($result);

		// 結果取得
		$tbl = array();
		while ($row = $result->fetchRow(MDB2_FETCHMODE_ASSOC)){
			array_push($tbl, $row);
		}

		// データ開放
		$result->free();

		return $tbl;
	}

	//////////////////////////////////////////////////
	// SELECTを実行（バインド変数利用）
	//////////////////////////////////////////////////
	function doSelect_b( $strSql, $param=array() ){

		// SELECT
		$sth = $this->db->prepare( $strSql );
		$result = $sth->execute( $param );

		// エラーチェック
		if( !$this->error_check( $result ) ){
			return false;
		}

		// テーブル情報取得
		$this->doTableInfo($result);

		// 結果取得
		$tbl = array();
		while ($row = $result->fetchRow(MDB2_FETCHMODE_ASSOC)){
			array_push($tbl, $row);
		}

		// データ開放
		$result->free();

		return $tbl;
	}

	//////////////////////////////////////////////////
	// クエリーの実行（バインド変数利用）
	//////////////////////////////////////////////////
	function doExecute($strSql, $param=array() ){

		// EXECUTE
		$sth = $this->db->prepare( $strSql );
		$result = $sth->execute( $param );

		// エラーチェック
		if( !$this->error_check( $result ) ){
			return false;
		}

		return true;
	}

##**************************************************************************
##  カラム情報
##**************************************************************************
	function doTableInfo($result){
		$this->db->loadModule('Reverse', null, true);
		$this->TABLE_INFO = $this->db->reverse->tableInfo( $result, null );
		//debug_print( $this->TABLE_INFO, 'TABLE_INFO' );
	}
	//カラム数
	function NumCols(){
		return count($this->TABLE_INFO);
	}
	//カラム名
	function ColumnName($i){
		return $this->TABLE_INFO[($i)]['name'];
	}
	//カラムタイプ
	function ColumnType($i){
		return $this->TABLE_INFO[($i)]['type'];
	}

##**************************************************************************
##  エラー処理
##**************************************************************************
	function error_check( $result ){
		if( MDB2::isError( $result ) ) {
			trigger_error( $result->getUserInfo() );
			//debug_print( $result->getUserInfo(), "PEAR ERROR" );
			return false;
		}
		return true;
	}
}
?>