<?
/*****************************************************************************
 *  name        : EpSession.inc
 *  function    : セッション支援クラス
 *
 *  create      : 2005/03/24
 ****************************************************************************/
class EpSession {
	var $session;

	/******************/
	/* コンストラクタ */
	/******************/
	function EpSession(){
		//セッションキャッシュの有効期限
		session_cache_expire( SESSION_CACHE_EXPIRE );

		//セッションキャッシュリミッタ設定
		session_cache_limiter( SESSION_CACHE_LIMITER );

		//セッションセーブpath
		session_save_path( SESSION_SAVE_PATH );

		//secureオプション
		//session_set_cookie_params( 0, SESSION_COOKIE_PATH, SESSION_DOMAIN_NAME, TRUE );

		//セッションスタート
		session_start();
	}

	//セッションに反映
	function sessionRegister($str){
		//session_register($str);
	}

	//セッションを破棄
	function sessionDestry(){
		session_unset();
		unset($_COOKIE[session_name()]);
		$_SESSION = array();
		session_destroy();
	}
}
?>
