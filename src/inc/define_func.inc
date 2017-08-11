<?php
/*****************************************************************************
 *  name        : define_func.inc
 *  function    : 関数定義
 *
 ****************************************************************************/
require_once( "DataDefineInfo.inc" );
$define_inf = new DataDefineInfo();

//////////////////////////////////////////////////////////////////////////////
// エラー関連
//////////////////////////////////////////////////////////////////////////////
ini_set( 'display_errors', 1 );					// エラー表示
ini_set( 'display_startup_errors', 1 );			// エラー表示(初期シーケンス時)
error_reporting( E_ALL );						// エラーレベルの設定
set_error_handler( "errorHandler" );			// エラーハンドラーの設定
register_shutdown_function( "EndFunction" );	// 終了処理関数の設定

function errorHandler ($errno, $errstr, $errfile, $errline){
	global $define_inf;

	// エラーレベル
	$levels = array(
		'1'=>'E_ERROR'
		,'2'=>'E_WARNING'
		,'4'=>'E_PARSE'
		,'8'=>'E_NOTICE'
		,'16'=>'E_CORE_ERROR'
		,'32'=>'E_CORE_WARNING'
		,'64'=>'E_COMPILE_ERROR'
		,'128'=>'E_COMPILE_WARNING'
		,'256'=>'E_USER_ERROR'
		,'512'=>'E_USER_WARNING'
		,'1024'=>'E_USER_NOTICE'
		,'2048'=>'E_STRICT'
		,'4096'=>'E_RECOVERABLE_ERROR'
		,'8192'=>'E_DEPRECATED'
		,'16384'=>'E_USER_DEPRECATED'
		,'30719'=>'E_ALL'
	);

	$catch_flg = 1;

	if( $errno == "8" ) { $catch_flg = 0; }
	if( $errno == "2048" ) { $catch_flg = 0; }
	if( $errfile == "/usr/local/lib/php/Smarty/sysplugins/smarty_resource.php" && $errline == "720" ) { $catch_flg = 0; } //Smarty 3.1.13
	if( $errfile == "/usr/share/php/Smarty/sysplugins/smarty_resource.php" && $errline == "772" ) { $catch_flg = 0; }
	if( $errno == "8192" && $errstr == "Assigning the return value of new by reference is deprecated" ) { $catch_flg = 0; }
	if(	$catch_flg == 1 ) {
		$define_inf->setErrno( $errno );
		$define_inf->setErrstr( $errstr );
		$define_inf->setErrfile( $errfile );
		$define_inf->setErrline( $errline );
		exit();
	}
}

function EndFunction() {
	if( DEBUG == "ON" ) {
		global $CtlSess;
		//$CtlSess->debug();
	}

	global $define_inf;
	global $main_tpl;

	$errno = $define_inf->getErrno();
	$errstr = $define_inf->getErrstr();
	$errfile = $define_inf->getErrfile();
	$errline = $define_inf->getErrline();
	$errsql = $define_inf->getErrSQL();
	$errbind = $define_inf->getErrBIND();
	// エラー時
	if( $errno != "0" && $errfile != "0" ) {
		if( DEBUG == "ON" ) {
			$template_list = array( 'ViewError.tpl' );
			// アサイン
		    $assign_data['titlename'] = '【エラー】';
			$assign_data['errno'] = $errno;
			$assign_data['errstr'] = $errstr;
			$assign_data['errfile'] = $errfile;
			$assign_data['errline'] = $errline;
			// サニタイジング
			$assign_data = doSanitize( $assign_data );
			// 表示
			print_r( $assign_data );
			print_r( $template_list );
			if( $errsql != "" ) {
				debug_print( $errsql, "SQL" );
			}
			if( $errbind != "" ) {
				debug_print( $errbind, "バインド" );
			}
		} else {
			// 何のエラーか表示しない(=システムエラー)
			print( '<font size="-1"><br>※申し訳ございません。只今メンテナンス中の為、サービスを停止しております。<br></font>' );
			exit();
		}
	}

}

//////////////////////////////////////////////////////////////////////////////
// アクセスカウンタ追加
//////////////////////////////////////////////////////////////////////////////
function AddAccessCnt() {
	$file_name = INC_PHP_PATH."AccessCnt.txt";
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

//////////////////////////////////////////////////////////////////////////////
// デバッグ表示関連
//////////////////////////////////////////////////////////////////////////////
// 変数からテーブル作成(横ver)
function debug_print( $array_value, $name = '' ){
	global $define_inf;
    $keys = array();
    $tab = '    ';
	$table_bgcolor = "#666666";
	$td_bgcolor = "#bbbbbb";

	if( DEBUG == "ON" ) {
	    if( isset($array_value[0]) && is_array( $array_value[0] ) ){
	        $keys = array_keys($array_value[0]);

	        $print_val = '<br><br><br><br><br><table align="center" width="90%" border="0" cellspancing="0" cellPadding="0" bgColor="'.$table_bgcolor.'">'."\n";
	        $print_val .= sprintf('<tr><td bgcolor="%s" colspan="%s"><font size="-1">%s</font></td></tr>', $td_bgcolor, count($keys), $name);

	        $print_val .= $tab.'<tr bgColor="#ffffff">';
	        foreach( $keys as $val ){
	            $print_val .= '<td bgcolor="'.$td_bgcolor.'" nowrap><font size="-1">'.$val.'</font></td>';
	        }
	        $print_val .= '</tr>'."\n";

	        foreach( $array_value as $k => $v) {
	            $print_val .= $tab.'<tr bgColor="#ffffff">';
	            foreach( $keys as $val ){
	                if( is_array( $v[$val] ) ){
	                    $print_val .= '<td><font size="-1">'.create_table_col_detail( $v[$val], $tab ).'</font></td>';
	                }else{
	                    $print_val .= '<td nowrap><font size="-1">'.conv_value($v[$val]).'</font></td>';
	                }
	            }
	            $print_val .= '</tr>'."\n";
	        }
	        $print_val .= '</table>'."\n";
	        $print_val .= '<br>'."\n";

	    }else{
	        $print_val = create_table_col( $array_value,  $name);
	    }
	}

	$define_inf->setPrintBuf( $print_val );
}

// 変数からテーブル作成(縦ver)
function create_table_col( $array_value, $name = '' ){
    $print_val = '';
    $tab = '    ';
	$table_bgcolor = "#666666";
	$td_bgcolor = "#bbbbbb";

    $print_val .= '<br><br><br><br><br><table align="center" width="90%" border="0" cellspancing="0" cellPadding="0" bgColor="'.$table_bgcolor.'">'."\n";
    $print_val .= $tab.'<tr><td bgcolor="'.$td_bgcolor.'" colspan="2"><font size="-1">'.$name.'</font></td></tr>'."\n";
    if( is_array( $array_value ) ){
        $print_val .= $tab.'<tr><td bgcolor="'.$td_bgcolor.'"><font size="-1">Key名</font></td><td bgcolor="'.$td_bgcolor.'"><font size="-1">値</font></td></tr>'."\n";
        foreach( $array_value as $k => $v) {
            $print_val .= $tab.'<tr bgColor="#ffffff"><td><font size="-1">'.$k.'</font></td><td><font size="-1">'.create_table_col_detail( $v, $tab ).'</font></td></tr>'."\n";
        }
    }else{
        if( is_scalar($array_value) ){
            $print_val .= $tab.'<tr bgColor="#ffffff"><td colspan="2"><font size="-1">'.conv_value( $array_value ).'</font></td></tr>'."\n";
        }else{
            $print_val .= $tab.'<tr bgColor="#ffffff"><td colspan="2"><font size="-1">&nbsp;</font></td></tr>'."\n";
        }
    }
    $print_val .= '</table>'."\n";
    $print_val .= '<br>'."\n";

    return $print_val;
}

// ハッシュ内容を編集
function create_table_col_detail( $array_value, $tab ){
    $tab2 = $tab.'    ';
	$table_bgcolor = "#666666";
	$td_bgcolor = "#bbbbbb";

    if( is_array( $array_value ) ){
        $tbl = "\n".$tab2.'<table border="0" cellspancing="0" cellPadding="0" width="100%" bgColor="'.$table_bgcolor.'">'."\n";
        $tbl .= $tab2.'<tr bgColor="#eeeeee"><td bgcolor="'.$td_bgcolor.'"><font size="-1">Key名</font></td><td bgcolor="'.$td_bgcolor.'"><font size="-1">値</font></td></tr>'."\n";
        foreach( $array_value as $k => $v) {
            $tbl .= $tab2.'    '.'<tr bgColor="#eeeeee"><td><font size="-1">'.$k.'</font></td><td><font size="-1">'.create_table_col_detail( $v, $tab2 ).'</font></td></tr>'."\n";
        }
        return $tbl.$tab2.'</table>'."\n".$tab;
    }else{
        if( is_scalar($array_value) ){
            return conv_value( $array_value );
        }else{
            return '&nbsp;';
        }
    }
}

// サニタイジング
function conv_value( $str ){
    $conv = $str;

    // 特殊文字をHTMLエンティティに変換
    $conv = htmlspecialchars( $conv );
    //
    $conv = str_replace( '	', '&nbsp;&nbsp;&nbsp;&nbsp;', $conv);
    // 半角スペース(' ')を&nbsp;に変換
    $conv = str_replace( ' ', '&nbsp;', $conv);
    // 改行(\n)を<br />に変換
    $conv = nl2br($conv);

    return $conv;
}

//////////////////////////////////////////////////////////////////////////////
// テンプレート関連
//////////////////////////////////////////////////////////////////////////////
/*
function template_assign( &$tpl, &$assign_data ){

	$tpl->assign( $assign_data );
}

function template_display( &$tpl, $tmplate_name = array() ){
	// テンプレート表示
	foreach( $tmplate_name as $val ){
		$tpl->display( $val );
	}
}

// テンプレファイルの削除
function template_rm() {
	$rm_files = DirFileGet( TEMPLATE_PATH."templates_c/" );
	//debug_print( $rm_files, "rm_files" );
	for ( $ix = 0 ; $ix < count( $rm_files ) ; $ix++ ) {
		unlink( TEMPLATE_PATH."templates_c/".$rm_files[$ix] );
	}
}
*/

//////////////////////////////////////////////////////////////////////////////
// サニタイジング
//////////////////////////////////////////////////////////////////////////////
function doSanitizeAll( &$data, $exception=array(), $def_key="" ){
    if( is_null($data) ){
        return;
    }

    if( is_array($data) ){
        foreach( $data as $key => $val ){
            doSanitizeAll( $val, $exception, $key );
            $data[$key] = $val;
        }
    }else{
        $def_key = str_replace( '*','\*', $def_key);
        $def_key = str_replace( '+','\+', $def_key);
        $def_key = str_replace( ')','\)', $def_key);
        $def_key = str_replace( '(','\(', $def_key);
        $def_key = str_replace( ']','\]', $def_key);
        $def_key = str_replace( '[','\[', $def_key);

        if( !preg_grep( "/$def_key/", $exception ) ){
            $data = str_replace( '&' , '&amp;' , $data);
            $data = str_replace( '\"' , '&quot;' , $data);
            $data = str_replace( '\\\'' , '&#39;' , $data);
            $data = str_replace( '<' , '&lt;' , $data);
            $data = str_replace( '>' , '&gt;' , $data);
            $data = str_replace( '\\\\' , '&yen;' , $data);
        }
    }
}

function doSanitize( $data, $exception=array(), $def_key="" ){
    doSanitizeAll( $data, $exception, $def_key );
    return $data;
}

//////////////////////////////////////////////////
// 配列のソート処理
//////////////////////////////////////////////////
function doArrayUsort( &$ary, $keys ){
	global $define_inf;
	$define_inf->set_env( 'sort_keys', $keys );
	$define_inf->set_env( 'sort_type', 1 );		// 昇順

	if( is_array($ary) && count($ary) >0 ){
		usort( $ary, "fnArrayCmp" );
	}
}
function doArrayUrsort( &$ary, $keys ){
	global $define_inf;
	$define_inf->set_env( 'sort_keys', $keys );
	$define_inf->set_env( 'sort_type', 2 );		// 降順

	if( is_array($ary) && count($ary) >0 ){
		usort( $ary, "fnArrayCmp" );
	}
}
function fnArrayCmp( $a, $b ){
	global $define_inf;
	$key = $define_inf->get_env( 'sort_keys' );
	$typ = $define_inf->get_env( 'sort_type' );

	$av  = getArrayValue( $a, $key );
	$bv  = getArrayValue( $b, $key );

	if ($av == $bv) return 0;
	if( $typ == 1 ){
		return ($av < $bv) ? -1 : 1;
	}else{
		return ($av < $bv) ? 1 : -1;
	}
}
function getArrayValue($ary, $key){
	if( is_array($key) ){
		$keys = array_keys($key);
		$key_value = $keys[0];

		return getArrayValue( $ary[$key_value], $key[$key_value] );
	}elseif(is_scalar($key)){
		return $ary[$key];
	}

	return 0;
}

//////////////////////////////////////////////////
// エンコード
//////////////////////////////////////////////////
// (SJIS to EUC)
function doSjis2Euc( $tbl ){
	if(is_array($tbl)){
		$tbl_enc = array();
		foreach($tbl as $key => $val ){
			$tbl_enc[doSjis2Euc($key)] = doSjis2Euc($val);
		}
	}else{
		set_time_limit(3000);
		$tbl_enc = mb_convert_encoding( $tbl, 'EUC-JP', 'SJIS' );
	}
	return $tbl_enc;
}
// (EUC to SJIS)
function doEuc2Sjis( $tbl ){
	if(is_array($tbl)){
		$tbl_enc = array();
		foreach($tbl as $key => $val ){
			$tbl_enc[doEuc2Sjis($key)] = doEuc2Sjis($val);
		}
	}else{
		$tbl_enc = mb_convert_encoding( $tbl, 'SJIS', 'EUC-JP' );
	}
	return $tbl_enc;
}
// (Auto to EUC)
function doAuto2Euc( $tbl ){
	if(is_array($tbl)){
		$tbl_enc = array();
		foreach($tbl as $key => $val ){
			$tbl_enc[doAuto2Euc($key)] = doAuto2Euc($val);
		}
	}else{
		set_time_limit(3000);
		$tbl_enc = mb_convert_encoding( $tbl, 'EUC-JP', 'auto' );
	}
	return $tbl_enc;
}
// (Auto to EUC)
function doAuto2Utf8( $tbl ){
	if(is_array($tbl)){
		$tbl_enc = array();
		foreach($tbl as $key => $val ){
			$tbl_enc[doAuto2Utf8($key)] = doAuto2Utf8($val);
		}
	}else{
		set_time_limit(3000);
		$tbl_enc = mb_convert_encoding( $tbl, 'UTF-8', 'auto' );
	}
	return $tbl_enc;
}
// (EUC to UTF-8)
function doEuc2Utf8( $tbl ){
	if(is_array($tbl)){
		$tbl_enc = array();
		foreach($tbl as $key => $val ){
			$tbl_enc[doEuc2Utf8($key)] = doEuc2Utf8($val);
		}
	}else{
		$tbl_enc = mb_convert_encoding( $tbl, 'UTF-8', 'EUC-JP' );
	}
	return $tbl_enc;
}
// (UTF-8 to EUC)
function doUtf82Euc( $tbl ){
	if(is_array($tbl)){
		$tbl_enc = array();
		foreach($tbl as $key => $val ){
			$tbl_enc[doUtf82Euc($key)] = doUtf82Euc($val);
		}
	}else{
		set_time_limit(3000);
		$tbl_enc = mb_convert_encoding( $tbl, 'EUC-JP', 'UTF-8' );
	}
	return $tbl_enc;
}
// (UTF-8 to SJIS)
function doUtf82Sjis( $tbl ){
	if(is_array($tbl)){
		$tbl_enc = array();
		foreach($tbl as $key => $val ){
			$tbl_enc[doUtf82Sjis($key)] = doUtf82Sjis($val);
		}
	}else{
		set_time_limit(3000);
		$tbl_enc = mb_convert_encoding( $tbl, 'SJIS', 'UTF-8' );
	}
	return $tbl_enc;
}
// (SJIS to UTF-8)
function doSjis2Utf8( $tbl ){
	if(is_array($tbl)){
		$tbl_enc = array();
		foreach($tbl as $key => $val ){
			$tbl_enc[doSjis2Utf8($key)] = doSjis2Utf8($val);
		}
	}else{
		set_time_limit(3000);
		$tbl_enc = mb_convert_encoding( $tbl, 'UTF-8', 'SJIS' );
	}
	return $tbl_enc;
}

//////////////////////////////////////////////////////////////////////////////
// 時刻変換
//////////////////////////////////////////////////////////////////////////////
function time_unix2win($unix_time) {
  //add the seconds between 1601-01-01 and 1970-01-01 and make it 100-nanosecond precision
  $win_time = ($unix_time + 11644473600) * 10000000;
  return $win_time;
}

function time_win2unix($win_time) {
  //round the win timestamp down to seconds and remove the seconds between 1601-01-01 and 1970-01-01
  $unix_time = round($win_time / 10000000) - 11644473600;
  return $unix_time;
}

//////////////////////////////////////////////////////////////////////////////
// ファイル一覧取得
//////////////////////////////////////////////////////////////////////////////
function DirFileGet( $dir_name ){
	// ファイル一覧を取得
	$dir_ary = array();
	//debug_print( $dir_name, "dir_name" );
	if( is_dir( $dir_name ) ) {
		if( $dh = opendir( $dir_name ) ) {
			while( ( $file = readdir($dh) ) !== false ) {
				if( !($file=='.' || $file=='..' ) ) {
					$dir_ary[] = $file;
				}
			}
			closedir($dh);
		}
	}

	$dir_ary = doSjis2Euc( $dir_ary ); // EUCに変換

	return $dir_ary;
}

//////////////////////////////////////////////////
// メール送信クラス
//////////////////////////////////////////////////
class clsMail
{
	var $to;
	var $from;
	var $cc;
	var $bcc;
	var $subject;
	var $message;
	var $contentType;

	var $mail_error;

	// コンストラクタ
	function clsMail(){
		$this->to		   =	"";
		$this->from		   =	"";
		$this->cc		   =	"";
		$this->bcc		   =	"";
		$this->subject	   =	"";
		$this->message	   =	"";
		$this->contentType = "html"; // text, html

		$this->mail_error  = "";
	}

	// メールアドレスチェック
	function isemail($email)
	{
	   // regx to test for valid e-mail adres
	   $regex = '/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]{2,})+$/';
	   if (preg_match($regex, $email)){
		   return true;
	   }else{
		   return false;
	   }
	}

	// メール送信
	function sendMail(){
		$mailtoary = array();
		$mailtoary = explode( ',', $this->to );
		foreach( $mailtoary as $val ){
			if(!$this->isemail($val)){
				$this->mail_error = "宛先が不正です。";
				return false;
			}
		}

		if(!$this->isemail($this->from)){
			$this->mail_error = "送信元が不正です。";
			return false;
		}

		if(!$this->cc==""){
			$mailccary = array();
			$mailccary = explode( ',', $this->cc );
			foreach( $mailccary as $val ){
				if(!$this->isemail($val)){
					$this->mail_error = "同報(CC)が不正です。";
					return false;
				}
			}
		}

		if(!$this->bcc==""){
			$mailbccary = array();
			$mailbccary = explode( ',', $this->bcc );
			foreach( $mailbccary as $val ){
				if(!$this->isemail($val)){
					$this->mail_error = "同報(BCC)が不正です。";
					return false;
				}
			}
		}

		## ヘッダー編集
		// メールタイプ判定 HTMLメール or TEXTメール
		$headers  = "MIME-Version: 1.0\r\n";
		if($this->contentType=="html"){
			$headers .= "Content-type: text/html; charset=iso-8859-1\r\n";
		}else{
			$headers .= "Content-type: text/plain; charset=us-ascii\r\n";
		}

		// CC, BCC
		$headers	 .= "From: ".$this->from."\r\n";
		if(!$this->cc==""){
			$headers .= "Cc: ".$this->cc."\r\n";
		}
		if(!$this->bcc==""){
			$headers .= "Bcc: ".$this->bcc."\r\n";
		}

		// メール送信
		return mb_send_mail ( $this->to, $this->subject, $this->message, $headers);
	}

	function debug(){
		return array(
			 'to' => $this->to
			,'from' => $this->from
			,'cc' => $this->cc
			,'bcc' => $this->bcc
			,'subject' => $this->subject
			,'message' => $this->message
			,'contentType' => $this->contentType
			,'mail_error' => $this->mail_error
		);
	}
}

//////////////////////////////////////////////////
// メール送信
//////////////////////////////////////////////////
function fnSendMail( $to, $subject, $body, $cc="", $bcc="", $type="text" ){
	$mail = new clsMail();
	$mail->to			= $to;
	$mail->from			= MAIL_FROM;
	$mail->subject		= $subject;
	$mail->message		= $body;
	$mail->cc			= $cc;
	$mail->bcc			= $bcc;
	$mail->contentType	= $type; // text, html

	$ret = $mail->sendMail();
	if(!$ret){
		debug_print( $mail->debug() );
		if( $mail->mail_error != "" ){
			trigger_error( $mail->mail_error );
		}
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////
// カレンダー
//////////////////////////////////////////////////////////////////////////////
function cal(){
	$result = '';
	$year = date(Y);
	$month = date(n);
	$result .= "<table border ='0' bgcolor='#5a4abd' cellspacing='1'><tr>\n";
	$result .= "<td colspan='7' bgcolor='white' align='center'><font size='-1'>".$year."年".$month."月</font></td><tr>";
	$result .= "<td style='text-align:center;background-color:#c0c0c0;font-size:10px;color:black;padding:2px'>日</td>\n";
	$result .= "<td style='text-align:center;background-color:#c0c0c0;font-size:10px;color:black;padding:2px'>月</td>\n";
	$result .= "<td style='text-align:center;background-color:#c0c0c0;font-size:10px;color:black;padding:2px'>火</td>\n";
	$result .= "<td style='text-align:center;background-color:#c0c0c0;font-size:10px;color:black;padding:2px'>水</td>\n";
	$result .= "<td style='text-align:center;background-color:#c0c0c0;font-size:10px;color:black;padding:2px'>木</td>\n";
	$result .= "<td style='text-align:center;background-color:#c0c0c0;font-size:10px;color:black;padding:2px'>金</td>\n";
	$result .= "<td style='text-align:center;background-color:#c0c0c0;font-size:10px;color:black;padding:2px'>土</td></tr>\n";

	$wtop = date(w,mktime(0,0,0,$month,1,$year)) ;  //指定月１日の曜日
	for($dot1 = 0 ; $dot1 < $wtop ; $dot1++){	   //日付が始まる前の空白
		$result .= "<td align='center'><font size='-1'>-</font></td>\n";
	}

	for( $day = 1 ; checkdate( $month, $day, $year ) ; $day++ ){		//日付記述
			$todaystamp = mktime(0,0,0,date(m),date(j),date(Y));
			$daystamp = mktime(0,0,0,$month,$day,$year);
		if(($day+$wtop)%7 == 1){
			if($todaystamp == $daystamp){
				$result .= "<tr>\n<td style='text-align:center;background-color:#bbbbff;font-size:10px;color:black;padding:2px'>$day</td>\n";	 //日曜日
			} else if($todaystamp > $daystamp) {
				$result .= "<tr>\n<td style='text-align:center;background-color:#ffffff;font-size:10px;color:red;padding:2px'>$day</td>\n";
			} else {
				$result .= "<tr>\n<td style='text-align:center;background-color:#ffffff;font-size:10px;color:red;padding:2px'>$day</td>\n";
			}
		} else if(($day+$wtop)%7 == 0){
			if($todaystamp == $daystamp){
				$result .= "<td style='text-align:center;background-color:#bbbbff;font-size:10px;color:blue;padding:2px'>$day</td>\n</tr>\n";		//土曜日
			} else if($todaystamp > $daystamp) {
				$result .= "<td style='text-align:center;background-color:#ffffff;font-size:10px;color:blue;padding:2px'>$day</td>\n</tr>\n";
			} else {
				$result .= "<td style='text-align:center;background-color:#ffffff;font-size:10px;color:blue;padding:2px'>$day</td>\n</tr>\n";
			}
		} else {
			if($todaystamp == $daystamp){
				$result .= "<td style='text-align:center;background-color:#bbbbff;font-size:10px;color:black;padding:2px'>$day</td>\n";		  //その他
			} else if($todaystamp > $daystamp) {
				$result .= "<td style='text-align:center;background-color:#ffffff;font-size:10px;color:black;padding:2px'>$day</td>\n";
			} else {
				$result .= "<td style='text-align:center;background-color:#ffffff;font-size:10px;color:black;padding:2px'>$day</td>\n";
			}
		}
	}

	$wend = date(w,mktime(0,0,0,$month,$day,$year)) ;   //指定月最終日の曜日$dayは３２になっている
	if($wend !=0){									  //  もし３２が日曜日すなわち０ならそれで終了
		for($dot2 =0 ; $dot2 < 7-$wend ;$dot2++){			   //０以外は・が必要
			$result .= "<td align='center'><font size='-1'></font></td>";
		}
	}
	$result .= "</tr></table>";

	return $result;
}

?>
