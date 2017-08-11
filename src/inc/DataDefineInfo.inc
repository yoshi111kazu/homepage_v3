<?php
/*****************************************************************************
 *  name        : DataDefineInfo.inc
 *  function    : ID情報
 *
 *  create      : 2005/04/01
 ****************************************************************************/

class DataDefineInfo {
	##初期変数
	var $print_tpl;		// 表示テンプレート
	var $print_buf;		// 表示バッファ
	var $errno;			// エラー番号
	var $errstr;		// エラー詳細
	var $errfile;		// エラーファイル
	var $errline;		// エラー行番号
	var $errsql;		// SQL文
	var $errbind;		// バインド変数

##**************************************************************************
##	コンストラクタ
##**************************************************************************
	function DataDefineInfo(){
		//初期化
		$this->print_tpl = "";
		$this->print_buf = "";
		$this->errno = 0;
		$this->errstr = "";
		$this->errfile = "";
		$this->errline = 0;
		$this->errsql = "";
		$this->errbind = "";
	}

##**************************************************************************
##	初期化処理
##**************************************************************************
	function initValue(){
		$this->DataDefineInfo();
	}

##**************************************************************************
##	getメソッド郡
##**************************************************************************
	function getPrintTpl(){
		return $this->print_tpl;
	}

	function getPrintBuf(){
		return $this->print_buf;
	}

	function getErrno(){
		return $this->errno;
	}

	function getErrstr(){
		return $this->errstr;
	}

	function getErrfile(){
		return $this->errfile;
	}

	function getErrline(){
		return $this->errline;
	}

	function getErrSQL(){
		return $this->errsql;
	}

	function getErrBIND(){
		return $this->errbind;
	}

	//全体の情報
	function getValue( $key=null, $opt=null ) {
		if( is_null($key) ) {
			$result = array();
			$result[print_tpl] = $this->getPrintTpl();
			$result[print_buf] = $this->getPrintBuf();
			$result[errno] = $this->getErrno();
			$result[errstr] = $this->getErrstr();
			$result[errfile] = $this->getErrfile();
			$result[errline] = $this->getErrline();
			$result[errsql] = $this->getErrSQL();
			$result[errbind] = $this->getErrBIND();
			return $result;
		} else if( is_scalar($key) ) {
			if($key == 'print_tpl'){ return $this->getPrintTpl(); }
			else if($key == 'print_buf'){ return $this->getPrintBuf(); }
			else if($key == 'errno'){ return $this->getErrno(); }
			else if($key == 'errstr'){ return $this->getErrstr(); }
			else if($key == 'errfile'){ return $this->getErrfile(); }
			else if($key == 'errline'){ return $this->getErrline(); }
			else if($key == 'errsql'){ return $this->getErrSQL(); }
			else if($key == 'errbind'){ return $this->getErrBIND(); }
		}
	}

    function get_env($key){
        return $this->env_value[$key];
    }
##**************************************************************************
##	setメソッド郡
##**************************************************************************
	function setPrintTpl( $val="") {
		if( is_scalar($val) ) {
			$this->print_tpl .= $val;
		}
	}

	function setPrintBuf( $val="") {
		if( is_scalar($val) ) {
			$this->print_buf .= $val;
		}
	}

	function setErrno( $val="") {
		if( is_scalar($val) ) {
			$this->errno = $val;
		}
	}

	function setErrstr( $val="" ) {
		if( is_scalar($val) ) {
			$this->errstr = $val;
		}
	}

	function setErrfile( $val="" ) {
		if( is_scalar($val) ) {
			$this->errfile = $val;
		}
	}

	function setErrline( $val="" ) {
		if( is_scalar($val) ) {
			$this->errline = $val;
		}
	}

	function setErrSQL( $val="" ) {
		if( is_scalar($val) ) {
			$this->errsql = $val;
		}
	}

	function setErrBIND( $val="" ) {
		if( is_scalar($val) ) {
			$this->errbind = $val;
		} else if( is_array($val) ) {
			$val_tmp = array_keys($val);
			foreach( $val_tmp as $key => $item ) {
				$this->errbind[$key] = $val[$item];
			}
		}
	}

	//全体の情報
	function setValue( $array ) {
		if( is_array($array) ){
			foreach( $array as $key=>$val ) {
				if( $key == "print_tpl" ) { $this->setPrintTpl( $val ); }
				if( $key == "print_buf" ) { $this->setPrintBuf( $val ); }
				if( $key == "errno" ) { $this->setErrno( $val ); }
				if( $key == "errstr" ) { $this->setErrstr( $val ); }
				if( $key == "errfile" ) { $this->setErrfile( $val ); }
				if( $key == "errline" ) { $this->setErrline( $val ); }
				if( $key == "errsql" ) { $this->setErrSQL( $val ); }
				if( $key == "errbind" ) { $this->setErrBIND( $val ); }
			}
		}
	}

    function set_env( $key, $val ){
        if( is_scalar($key) ){
            $this->env_value[$key] = $val;
        }
    }
}
?>
