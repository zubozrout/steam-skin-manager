/* Steam Skin Manager HELP section Javascript */

window.onload = function() {
	$ul = document.getElementById("menu");
	if($ul) {
		$list = $ul.getElementsByTagName("li");
		
		for($i = 0; $i < $list.length; $i++) {
			$list[$i].style.backgroundColor = "#" + ($list.length - $i) + $i + "8";
			$list[$i].style.boxShadow = "0 0 8px #" + ($list.length - $i) + $i + "8";
			$list[$i].className = "altered";
			
			$children = $list[$i].getElementsByTagName("h2");
			for($j = 0; $j < $children.length; $j++) {
				$children[$j].style.color = "#fff";
			}
			
			$children = $list[$i].getElementsByTagName("span");
			for($j = 0; $j < $children.length; $j++) {
				$children[$j].style.color = "#ddd";
			}
			
			if(($i + 1) == $list.length) {
				document.body.style.backgroundColor = "#" + ($list.length - $i + 2) + ($i + 1) + "a";
				document.getElementsByTagName("h1")[0].style.color = "#fff";
			}
		}
	}
}
