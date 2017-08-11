<?php
/*****************************************************************************
 *  Name        : DataIdInfo.inc
 *  function    : ID情報
 *
 *  create      : 2005/04/01
 ****************************************************************************/

class DataIdInfo {
	// 初期変数
	var $User;	// ユーザー名
	var $Pass;	// パスワード
	var $Name;	// ユーザーの名前
	var $ID;	// ユーザーID
	var $GID;	// GID

##**************************************************************************
##	コンストラクタ
##**************************************************************************
	function DataIdInfo(){
		//初期化
		$this->User = array();
		$this->Pass = array();
		$this->Name = array();
		$this->ID = array();
		$this->GID = array();
	}

##**************************************************************************
##	初期化
##**************************************************************************
	function initValue(){
		$this->DataIdInfo();
	}

##**************************************************************************
##	データ
##**************************************************************************
///////////////////////////////////////////////////////////////////////////////
// GET
///////////////////////////////////////////////////////////////////////////////
	// 全体の情報
	function getValue( $key=null ) {
		if( is_null($key) ) {
			$result = array();
			$result['User'] = $this->getUser();
			$result['Pass'] = $this->getPass();
			$result['Name'] = $this->getName();
			$result['ID'] = $this->getID();
			$result['GID'] = $this->getGID();
			return $result;
		} else if( is_scalar($key) ) {
			if($key == 'User'){ return $this->getUser(); }
			else if($key == 'Pass'){ return $this->getPass(); }
			else if($key == 'Name'){ return $this->getName(); }
			else if($key == 'ID'){ return $this->getID(); }
			else if($key == 'GID'){ return $this->getGID(); }
		}
	}

	// ユーザー名
	function getUser(){
		return $this->User[value];
	}

	// パスワード
	function getPass(){
		return $this->Pass[value];
	}

	// ユーザーの名前
	function getName(){
		return $this->Name[value];
	}

	// ユーザーID
	function getID(){
		return $this->ID[value];
	}

	// GID
	function getGID(){
		return $this->GID[value];
	}

///////////////////////////////////////////////////////////////////////////////
// SET
///////////////////////////////////////////////////////////////////////////////
	// 全体の情報
	function setValue( $array ) {
		if( is_array($array) ){
			foreach( $array as $key=>$val ) {
				if( $key == "User" ) { $this->setUser( $val ); }
				if( $key == "Pass" ) { $this->setPass( $val ); }
				if( $key == "Name" ) { $this->setName( $val ); }
				if( $key == "ID" ) { $this->setID( $val ); }
				if( $key == "GID" ) { $this->setGID( $val ); }
			}
		}
	}

	// ユーザー名
	function setUser( $val="") {
		if( is_scalar($val) ) {
			$this->User[value] = $val;
		}
	}

	// パスワード
	function setPass( $val="" ) {
		if( is_scalar($val) ) {
			$this->Pass[value] = $val;
		}
	}

	// ユーザーの名前
	function setName( $val="" ) {
		if( is_scalar($val) ) {
			$this->Name[value] = $val;
		}
	}

	// ユーザーID
	function setID( $val="" ) {
		if( is_scalar($val) ) {
			$this->ID[value] = $val;
		}
	}

	// GID
	function setGID( $val="" ) {
		if( is_scalar($val) ) {
			$this->GID[value] = $val;
		}
	}

##**************************************************************************
##	エラー
##**************************************************************************
///////////////////////////////////////////////////////////////////////////////
// エラーチェック
///////////////////////////////////////////////////////////////////////////////
	function isError(){
		$error = $this->getError();
		foreach($error as $key=>$val){
			if($val){
				return TRUE;
			}
		}
	}

///////////////////////////////////////////////////////////////////////////////
// GET
///////////////////////////////////////////////////////////////////////////////
	function getError(){
		$result = array();
		$result[User] = $this->User[error];
		$result[Pass] = $this->Pass[error];
		return $result;
	}

///////////////////////////////////////////////////////////////////////////////
// SET
///////////////////////////////////////////////////////////////////////////////
	// ユーザー名
	function setUserError($code){
		if($code){
			$this->User[error] = $code;
		}
	}

	// パスワード
	function setPassError($code){
		if($code){
			$this->Pass[error] = $code;
		}
	}

##**************************************************************************
##	データチェック
##**************************************************************************
	// 全体チェック
	function valueCheck(){
		$this->checkUser();
		$this->checkPass();
	}

	// ユーザー名
	function checkUser(){
		// 取得
		$User[value] = trim( $this->getUser() );

		// 半角英数以外
		if( !preg_match("/[a-z0-9]/", $User[value]) ) {
			$this->setUserError(3);
		}

		// 20文字以内
		if( strlen($User[value]) > 20 ) {
			$this->setUserError(2);
		}

		// 未入力
		if( $User[value] == "" ) {
			$this->setUserError(1);
		}
	}

	// パスワード
	function checkPass(){
		// 取得
		$Pass[value] = trim( $this->getPass() );

		// 半角英数以外
		//if( !preg_match("/[a-z0-9]/", $Pass[value]) ) {
			//$this->setPassError(3);
		//}

		// 20文字以内
		if( strlen($Pass[value]) > 20 ) {
			$this->setPassError(2);
		}

		// 未入力
		if( $Pass[value] == "" ) {
			$this->setPassError(1);
		}
	}
}
?>
