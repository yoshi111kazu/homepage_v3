<?
/*****************************************************************************
 *  name        : MemberClass.inc
 *  function    : 会員情報取得(DB,FILEバージョン)
 *
 *  create      : 2005/03/31
 ****************************************************************************/
class MemberClass {
	var $db;

//////////////////////////////////////////////////////////////////////////////
// コンストラクタ
//////////////////////////////////////////////////////////////////////////////
	function MemberClass(){
	}

//////////////////////////////////////////////////////////////////////////////
// 会員情報取得(DB)
//////////////////////////////////////////////////////////////////////////////
	function getMember_D( $user, $pass, $access_cnt ){
		require_once( INC_DB_PATH."CtlDataBase_MDB2_my.inc" );
		//debug_print( $access_cnt, "access_cnt" );

		// admin
		if ( $user == ADMIN_USER ) {
			$split_data = array();
			$split_data = explode( '_', $pass );
			$pass = $split_data[0];
			$pass_2 = $split_data[1];
		}

		// 接続
		$db = new CtlDataBase( INC_DB_PATH."define_db_my.inc" );
		$db->connectDatabase();

		// バインド
		$bind = array();
		$bind[] = $user;
		$bind[] = $pass;

		// SQL文
		$str_sql = <<<EOF
			select
				user_id,
				group_id,
				disp_name
			from
				u_user
			where
					enabled_flag = 'Y'
				and user_name = ?
				and pass = password( ? )
EOF;
		//debug_print( $str_sql, "str_sql" );
		//debug_print( $bind, "bind" );
		// 実行
		$result = $db->doSelect_b( $str_sql, $bind );
		//debug_print( $result, "result" );

		// 切断
		$db->disconnectDatabase();

		if ( count( $result ) == 1 && $result[0]['user_id'] >= 1 ) {

			// admin
			if ( $user == ADMIN_USER ) {
				$pass_cnt = substr( $access_cnt, strlen( $access_cnt ) - 1, 1 );
				//debug_print( $pass_cnt, "pass_cnt" );
				$pass_2_phraze = date( 'm' ) * date( 'd' ) + $pass_cnt;
				if ( $result[0]['user_id'] == 1 && $pass_2 == $pass_2_phraze ) {
					// ok
				} else {
			    	return -1;
				}
			}

			$result_tmp = array();
			$result_tmp[0] = $result[0]['user_id'];
			$result_tmp[1] = $result[0]['group_id'];
			$result_tmp[2] = $result[0]['disp_name'];
	    	return $result_tmp;
		} else {
	    	return -1;
		}
	}

//////////////////////////////////////////////////////////////////////////////
// 会員情報取得(FILE)
//////////////////////////////////////////////////////////////////////////////
	function getMember_F( $user, $pass ){
		// ファイルオープン
		$fp = fopen( PASS_FILE, "r" );

		while ( !feof( $fp ) ) {
			// 読み込み
			$read_line = fgets( $fp );
			$read_line = doSjis2Utf8( $read_line ); // UTFに変換
			// 判定
			$splt_data = explode( ';', $read_line );
			if ( $splt_data[2] == $user && $splt_data[3] == $pass ) {
				// ファイルクローズ
				fclose( $fp );
				return $splt_data;
			}
		}

		// ファイルクローズ
		fclose( $fp );
		return -1;
	}

}
?>