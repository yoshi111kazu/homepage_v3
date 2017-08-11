<?
/*****************************************************************************
 *  name        : CtlSession.inc
 *  function    : セッション支援クラス
 *
 *  create      : 2005/03/24
 ****************************************************************************/

class CtlSession
{
	var $session;		// セッションクラス
	var $id;			// ユーザー情報
	var $anyData;		// 汎用情報

##**************************************************************************
##	コンストラクタ
##**************************************************************************
	function CtlSession(){
		/******************/
		/* Cookie有効設定 */
		/******************/
		ini_set('session.use_only_cookies', 1);

		/*****************************/
		/* session制御クラス読み込み */
		/*****************************/
		require_once( "EpSession.inc" );
		$this->session = new EpSession($this);

		/********************/
		/* オブジェクト生成 */
		/********************/
		// ID情報クラス
		require_once( INC_SES_PATH."DataIdInfo.inc" );
		$this->id = new DataIdInfo();
		// 汎用情報クラス
		require_once( INC_SES_PATH."DataAny.inc" );
		$this->anyData = new DataAny();

		/************************************/
		/* セッション値をオブジェクトに反映 */
		/************************************/
		$this->id->setValue( $_SESSION['id'] );
		$this->anyData->setValue( $_SESSION['anyData'] );

		/**************/
		/* POST取得時 */
		/**************/
		if( isset($_POST['id']) ) {
			$this->id->initValue();
			$this->id->setValue( $_POST );
			$this->id->valueCheck();
		}
	}

##**************************************************************************
##	isメソッド関連
##**************************************************************************
	// クッキーチェック
	function isCookie(){
        $session_name = session_name();
        if( $_COOKIE[$session_name]==session_id() ){
            return true;
        }
        return false;
	}

	// 管理者チェック
	function isAdmin(){
		if ( $this->id->getID() != ADMIN_ID ) {
			header( "Location: index.html" );
			exit();
		}
	}

	// Wikiチェック
	function isWiki(){
		if ( $this->id->getID() != ADMIN_ID ) {
			header( "Location: computer.html" );
			exit();
		}
	}

	// SAIDAIチェック
	function isSaidai(){
		if ( $this->id->getGID() != SAIDAI_GROUP_ID ) {
			header( "Location: index.html" );
			exit();
		}
	}

	// ログインチェック
	function isLogin(){
		if ( $this->id->getID() == '' ) {
			header( "Location: index.html" );
			exit();
		}
	}

##**************************************************************************
##	セッション関連
##**************************************************************************
	// セッション設定
	function sessionRegister(){
		$_SESSION['id'] = $this->id->getValue();
		$_SESSION['anyData'] = $this->anyData->getValue();
	}

    // セッション破棄
	function destroy(){
		$this->session->sessionDestry();
	}

##**************************************************************************
##	デバッグ関連
##**************************************************************************
	function debug(){
		global $define_inf;

	    // デバッグ
		function output($key, $data, $color="#cccccc"){
			$buf = "";
			$buf.='<tr>'."\n";
			$buf.='<td bgcolor="'.$color.'">'."\n";
			$buf.=$key;
			$buf.='</td>'."\n";
			if(is_array($data)){
				$buf.='<td width="100%">'."\n";
				$buf.='<table border="1">'."\n";
				foreach($data as $key2=>$val2){
					$buf.=output($key2, $val2, $color);
				}
				$buf.='</table>'."\n";
				$buf.='</td>'."\n";
			}else{
				$buf.='<td width="100%">'."\n";
				if($data == ""){
					$buf.='<font color="#aaaaaa" size="-1">--- Data is Empty ---</font>' . "\n";
				}else{
					$buf.=$data . "\n";
				}
				$buf.='</td>'."\n";
				$buf.='</td>'."\n";
			}
			$buf.='</tr>'."\n";
			return $buf;
		}

		// デバッグ出力
		if( DEBUG == "ON" ) {
			// セッション情報
			$dbg_ses = array();
			$session_name = session_name();
			$dbg_ses['COOKIE'] = $_COOKIE[$session_name];
			$dbg_ses['SESSION_ID'] = session_id();

			$buf='<div align="center">';

			$buf.='<table width="700" bgcolor="#003300">';
			$buf.='<tr><td align="left"><font color="#ffffff" size="-1">SESSION_ID => '.$dbg_ses['SESSION_ID'].' </font></td></tr>';
			$buf.='</table>';

			/***********/
			/* SESSION */
			/***********/
			// ID
			$buf.='<table width="700" bgcolor="#003300">';
			$buf.='<tr><td align="center"><font color="#ffffff" size="-1"><b>$_SESSION[&nbsp;ID&nbsp;]</b></font></td></tr>';
			$buf.='</table>';
			$buf.='<table width="700" bgcolor="#99CC99">';
			$buf.='<tr><td align="center"><font color="#003300" size="-1"><b>DATA</b></font></td></tr>';
			$buf.='</table>';
			$buf.='<table width="700" border="1">';
			$buf.=output('',$_SESSION['id'],"#99CC99");
			$buf.='</table>';
			$buf.='<br>';
			// anyData
			$buf.='<table width="700" bgcolor="#003300">';
			$buf.='<tr><td align="center"><font color="#ffffff" size="-1"><b>$_SESSION[&nbsp;anyData&nbsp;]</b></font></td></tr>';
			$buf.='</table>';
			$buf.='<table width="700" bgcolor="#99CC99">';
			$buf.='<tr><td align="center"><font color="#003300" size="-1"><b>DATA</b></font></td></tr>';
			$buf.='</table>';
			$buf.='<table width="700" border="1">';
			$buf.=output('',$_SESSION['anyData'],"#99CC99");
			$buf.='</table>';
			$buf.='<br>';

			/*********/
			/* CLASS */
			/*********/
			// ID
			$buf.='<table width="700" bgcolor="#000000">';
			$buf.='<tr><td align="center"><font color="#ffffff" size="-1"><b>ID DATA</b></font></td></tr>';
			$buf.='</table>';
			$buf.='<table width="700" bgcolor="#999999">';
			$buf.='<tr><td align="center"><font color="#000000" size="-1"><b>DATA</b></font></td></tr>';
			$buf.='</table>';
			$buf.='<table width="700" border="1">';
			$buf.=output('', $this->id->getValue());
			$buf.='</table>';
			$buf.='<table width="700" bgcolor="#999999">';
			$buf.='<tr><td align="center"><font color="#000000" size="-1"><b>ERROR CHECK</b></font></td></tr>';
			$buf.='</table>';
			$buf.='<table width="700" border="1">';
			$buf.=output('', $this->id->getError());
			$buf.='</table>';
			$buf.='<br>';
			// ID
			$buf.='<table width="700" bgcolor="#000000">';
			$buf.='<tr><td align="center"><font color="#ffffff" size="-1"><b>anyData DATA</b></font></td></tr>';
			$buf.='</table>';
			$buf.='<table width="700" bgcolor="#999999">';
			$buf.='<tr><td align="center"><font color="#000000" size="-1"><b>DATA</b></font></td></tr>';
			$buf.='</table>';
			$buf.='<table width="700" border="1">';
			$buf.=output('', $this->anyData->getValue());
			$buf.='</table>';
			$buf.='<br>';

			$define_inf->setPrintBuf( $buf );
		}
	}
}
?>
