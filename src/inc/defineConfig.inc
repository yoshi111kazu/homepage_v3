<?
	// テンプレートファイルの削除
	//template_rm();

	/**********/
	/* Smarty */
	/**********/
	//require_once( SMARTY_PHP );
	//$main_tpl = new Smarty();
	//$main_tpl->setTemplateDir = TEMPLATE_PATH."templates";
	//$main_tpl->setCompileDir = TEMPLATE_PATH."templates_c";
	//$main_tpl->compile_check = true;
	//$main_tpl->caching = 0;

	/***********/
	/* Session */
	/***********/
	require_once( INC_SES_PATH."CtlSession.inc" );
	$CtlSess = new CtlSession();

	/***************/
	/* 初期アサイン */
	/***************/
	// ログイン者
	$assign_before_data = array();
	$assign_before_data['login_name'] = $_SESSION['id']['Name'];
	$assign_before_data['login_id'] = $_SESSION['id']['ID'];
	$assign_before_data['group_id'] = $_SESSION['id']['GID'];
	// アクセス数
	$NewCnt = AddAccessCnt();
	$CtlSess->anyData->setValue( 'access_cnt', $NewCnt );
	$CtlSess->sessionRegister();
	$assign_before_data['access_cnt'] = $CtlSess->anyData->getValue( 'access_cnt' );
	//template_assign( $main_tpl, $assign_before_data );

	/*********/
	/* Debug */
	/*********/
	if ( $CtlSess->id->getID() == ADMIN_ID ) {
		define( 'DEBUG' , 'ON');
	}
	//debug_print( $_POST, "POST" );
	//$CtlSess->debug();
	//debug_print( $assign_before_data, "assign_before_data" );
?>
