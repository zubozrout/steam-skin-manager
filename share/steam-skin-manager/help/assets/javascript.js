/* Steam Skin Manager HELP section Javascript */

window.onload = function() {
	$ul = document.getElementById("menu");
	if($ul) {
		$list = $ul.getElementsByTagName("li");
		
		for($i = 0; $i < $list.length; $i++) {
			$Bcol = "f";
			$Tcol = "f";
			$TLcol = "f";
			switch($i) {
				case 0:
					$Bcol = "e";
					$Tcol = "3";
					$TLcol = "8";
					break;
				case 1:
					$Bcol = "d";
					$Tcol = "3";
					$TLcol = "7";
					break;
				case 2:
					$Bcol = "c";
					$Tcol = "3";
					$TLcol = "6";
					break;
				case 3:
					$Bcol = "b";
					$Tcol = "2";
					$TLcol = "5";
					break;
				case 4:
					$Bcol = "a";
					$Tcol = "0";
					$TLcol = "4";
					break;
				default:
					$Bcol = 14 - $i;
					if($Bcol < 0) {
						$Bcol = 0;
					}
					$Tcol = "e";
					$TLcol = "d";
			}
			$list[$i].style.backgroundColor = "#" + $Bcol + $Bcol + $Bcol;
			$list[$i].className = "altered";
			
			$children = $list[$i].getElementsByTagName("h2");
			for($j = 0; $j < $children.length; $j++) {
				$children[$j].style.color = "#" + $Tcol + $Tcol + $Tcol;
			}
			
			$children = $list[$i].getElementsByTagName("span");
			for($j = 0; $j < $children.length; $j++) {
				$children[$j].style.color = "#" + $TLcol + $TLcol + $TLcol;
			}
		}
	}
}
