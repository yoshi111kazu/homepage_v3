<?php
/*****************************************************************************
 *  name        : BBS.inc
 *  function    : BBSクラス
 *
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
// BBSクラス
//////////////////////////////////////////////////////////////////////////////
class ClsBBS {

	/******************/
	/* コンストラクタ */
	/******************/
	function ClsBBS() {
	}

	/******************/
	/* BBSの取得      */
	/******************/
	function BBSFileGet( $bbs_top, $filter='all' ) {
		// ファイル一覧を取得
		$path =  BBS_PATH.$bbs_top."/txt";
		$dir_ary = array();
		$dir_ary = DirFileGet( $path );
		//debug_print( $dir_ary, "dir_ary" );

		// データ取得
		$result = array();
		for( $ix = 0 ; $ix < count( $dir_ary ) ; $ix++ ) {
			// ファイルチェック
			if( preg_match( "/^[0-9]{14}_.*\.txt$/", $dir_ary[$ix] ) ) {
				$str_tmp = explode( '_', $dir_ary[$ix] );
				$str_time[0] = substr( $str_tmp[0], 0, 4 );
				$str_time[1] = substr( $str_tmp[0], 4, 2 );
				$str_time[2] = substr( $str_tmp[0], 6, 2 );
				$str_time[3] = substr( $str_tmp[0], 8, 2 );
				$str_time[4] = substr( $str_tmp[0], 10, 2 );
				$str_time[5] = substr( $str_tmp[0], 12, 2 );
				$str_cnt = $str_tmp[1];
				$file_flag = str_replace( '.txt', '', $str_tmp[2] );
				if ( $filter == 'file' ) {
					if ( $file_flag == 0 ) {
						continue;
					}
				}
				// ファイル内容取得
				$fl_ary = file( $path."/".$dir_ary[$ix] );
				$fl_ary = doSjis2Utf8( $fl_ary );	// EUCに変換
				$str_nickname = $fl_ary[0];			// nickname
				$str_file = rtrim( $fl_ary[1] );	// ファイル名
				$str_file_dsp = rtrim( $fl_ary[2] );// ファイル名(ユーザ,表示用)
				// ファイルサイズ
				$file_size = '';
				if ( $file_flag > 0 ) {
					$full_file_path = BBS_PATH.$bbs_top."/file/".$str_file;
					if ( file_exists( $full_file_path ) ) {
						$file_size = round( ( filesize( $full_file_path ) / 1024 ), 1 );
					}
				}
				// Body
				array_shift( $fl_ary );
				array_shift( $fl_ary );
				array_shift( $fl_ary );

				$result[] = array(
									'cnt' => $str_cnt,
									'time' => $str_tmp[0],
									'time_dsp' => $str_time[0]."/".$str_time[1]."/".$str_time[2]." ".$str_time[3].":".$str_time[4].":".$str_time[5],
									'nick_name' => $str_nickname,
									'file_flag' => $file_flag,
									'file_name' => $str_file,
									'file_name_dsp' => $str_file_dsp,
									'file_size' => $file_size,
									'body' => $fl_ary
								);
			}
		}

		doArrayUrsort( $result, 'cnt' ); // ソート

		return $result;
	}

	/**********************/
	/* パラメータチェック */
	/**********************/
	function ParamCheck( $bbs_top, $body, $nickname, $file=null ){

		//debug_print( $body.$nickname, "body:nickname" );
		// Top
		if ( $bbs_top == 'top' ) {
			if ( $nickname == '' || $body == '' ) {
				return( '未入力' );
			}
		// saidai
		} else if ( $bbs_top == 'saidai' ) {
			if ( $nickname == '' || $body == '' ) {
				return( '未入力' );
			}
			//debug_print( $file['userfile']['name'], "userfile_name" );
			if ( $file['userfile']['name'] != '' ) {
				if ( $file['userfile']['size'] == 0 || $file['userfile']['error'] != 0 ) {
					if ( $file['userfile']['error'] == 1 ) {
						return( 'ファイルサイズエラー' );
					} else {
						return( 'ファイルエラー('.$file['userfile']['error'].')' );
					}
				}
			}
		}

		return '';
	}

	/******************/
	/* ログの作成     */
	/******************/
	function MakeLogFile( $bbs_top, $body, $nickname, $file=null ){
		$str_time = date( 'YmdHis' );		// 時刻の取得
		$str_cnt = $this->GetLogCnt( BBS_PATH.$bbs_top );	// カウントの取得

		if ( preg_match("/^[ぁ-ん]+$/", $body ) ) {

			// FILE関連
			// 0 : 添付なし
			// 1 : 画像
			// 2 : その他ファイル形式
			$file_flg = 0;
			if ( isset( $file['userfile']['name'] ) ) {
				$user_file_name = $file['userfile']['name'];
				if ( $file['userfile']['name'] != '' ) {
					if ( preg_match( "/\.jpg$|\.jpeg$|\.gif$|\.png$/", $user_file_name ) ) {
						$file_flg = 1;
					} else {
						$file_flg = 2;
					}
					$ext = explode( '.', $user_file_name );
					$put_file_name = $str_time."_".$str_cnt.".".$ext[1];
					move_uploaded_file( $file['userfile']['tmp_name'], BBS_PATH.$bbs_top."/file/".$put_file_name );
				}
			}

			// Logファイル名作成
			$txt_name = BBS_PATH.$bbs_top."/txt/".$str_time."_".$str_cnt."_".$file_flg.".txt";

			// 本文作成
			$str_put = doUtf82Sjis( $nickname."\n".$put_file_name."\n".$user_file_name."\n".$body );
			$fp = fopen( $txt_name, "w" ); // ファイルオープン
			fputs( $fp, $str_put ); // 書き込み
			fclose( $fp ); // ファイルクローズ
		}

		return true;
	}

	/********************/
	/* ログカウント更新 */
	/********************/
	function GetLogCnt( $path ){
		$file_name = $path."/Cnt.txt";

		// ファイルオープン
		$fp = fopen( $file_name, "r" );
		flock( $fp, LOCK_SH );
		$Cnt = fgets( $fp );
		flock( $fp, LOCK_UN );
		// ファイルクローズ
		fclose( $fp );

		$Cnt_new = $Cnt + 1;
		// ファイルオープン
		$fp = fopen( $file_name, "w" );
		fputs( $fp, $Cnt_new );
		flock( $fp, LOCK_UN );
		// ファイルクローズ
		fclose( $fp );

		return $Cnt_new;
	}

	/**************************/
	/* 添付ファイルの一覧取得 */
	/**************************/
	function GetUserFileList( $bbs_top ) {

		// ファイル一覧取得
		$file_aly = array();
		$file_aly = DirFileGet( BBS_PATH.$bbs_top."/txt" );

		$result = array();
		foreach ( $file_aly as $key => $val ) {
			$splt_tmp_01 = array(); $splt_tmp_02 = array();
			$splt_tmp_01 = explode( '_', $val );
			$splt_tmp_02 = explode( '.', $splt_tmp_01[2] );
			if ( $splt_tmp_02[0] == 1 || $splt_tmp_02[0] == 2 ) {
				$file_body_aly = array();
				$file_body_aly = file( BBS_PATH.$bbs_top."/txt/".$val );
				$file_body_aly = doSjis2Utf8( $file_body_aly );
				$result[] = array( 'file_name' => $file_body_aly[1],
									'file_name_dsp' => $file_body_aly[2]
								);
			}
		}

		doArrayUrsort( $result, 'file_name' ); // ソート

		return $result;
	}

	/************************/
	/* ファイルダウンロード */
	/************************/
	function FileDownload( $bbs_top, $file_path, $user_file_name='no_name' ) {

		$file_path = BBS_PATH.$bbs_top."/file/".$file_path;
		// ファイルの存在確認
		if ( file_exists( $file_path ) ) {
			// オープンできるか確認
		    if ( ( $fp = fopen( $file_path, "r" ) ) ) {
				fclose( $fp ); // クローズ
			    // ファイルサイズの確認
				if ( ( $content_length = filesize( $file_path ) ) > 0 ) {
					// ダウンロード用のHTTPヘッダ送信
					header( "Content-Disposition: inline; filename=\"".basename( $user_file_name )."\"" );
					header( "Content-Length: ".$content_length );
					header( "Content-Type: application/octet-stream" );
			    	readfile( $file_path );
				}
			}
		}

	}
}
?>
