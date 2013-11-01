/*********************************************
 * SmartHome tree menu by
 * 		Q.Bryant <qbing2010@163.com>
 *
 * Copyright 2013  Q.Bryant
 *
 * DATA:
 *		2013-09-16    20:15:03
 ********************************************/

var curMenu = null, zTree_Menu = null;
var zTree, rMenu;
var setting = {
	view: {
		showLine: true,
		selectedMulti: false,
		dblClickExpand: false
	},
	data: {
		simpleData: {
			enable: true
		}
	},
	callback: {
		onNodeCreated: this.onNodeCreated,
		beforeClick: this.beforeClick,
		onClick: this.onClick,
		onRightClick: OnRightClick
	}
};
var old_zNodes_str = '';
var zNodes =[{name:"smarthome"}];
function beforeClick(treeId, node) {
	if (node.isParent) {
		zTree_Menu.expandNode(node);
	}
	return !node.isParent;
}
function strToJson(str){ 
	var json = eval('(' + str + ')'); 
	return json; 
} 

function refresh()  
{  
	$.get(
		"/SmartHomeView.html",
		function(data,status){						
			JsonTreeText = "" +data;
			JsonTreeText = JsonTreeText.replace(/NAME/ig, "name");
			JsonTreeText = JsonTreeText.replace(/CHILD/ig, "children");

			if(JsonTreeText!=old_zNodes_str){
				zNodes = strToJson(JsonTreeText);
				showSmartHomeView();
				old_zNodes_str = JsonTreeText;
				$("#formate_jsom_str").html(old_zNodes_str);
			}
		}
	);	
	setTimeout("refresh();",10000);  
}

function showSmartHomeView()
{
	$.fn.zTree.init($("#treeDemo"), setting, zNodes);
	zTree_Menu = 	$.fn.zTree.getZTreeObj("treeDemo");
	zTree = 		$.fn.zTree.getZTreeObj("treeDemo");
	rMenu = 		$("#rMenu");
	
	curMenu = zTree_Menu.getNodes()[0];
	zTree_Menu.selectNode(curMenu);
	var a = $("#" + zTree_Menu.getNodes()[0].tId + "_a");
	a.addClass("cur");		
}

$(document).ready(function(){
	showSmartHomeView();
	refresh();
});

var test = 1;
function OnRightClick(event, treeId, treeNode) {
	if(!treeNode.isParent){
		if (!treeNode && event.target.tagName.toLowerCase() != "button" && $(event.target).parents("a").length == 0) {
			zTree.cancelSelectedNode();
			showRMenu("root", event.clientX, event.clientY);
			test = 0 ;
		} else if (treeNode && !treeNode.noR) {
			zTree.selectNode(treeNode);
			showRMenu("node", event.clientX, event.clientY);
			test = 1;
		}
	}
}


function showRMenu(type, x, y) {
	//alert("showRMenu");
	$("#rMenu ul").show();
	if (type=="root") {
		$("#m_close").hide();
		$("#m_help").hide();
	} else {
		$("#m_close").show();
		$("#m_help").show();
	}
	rMenu.css({"top":y+"px", "left":x+"px", "visibility":"visible"});

	$("body").bind("mousedown", onBodyMouseDown);
}
function hideRMenu() {
	if (rMenu) rMenu.css({"visibility": "hidden"});
	$("body").unbind("mousedown", onBodyMouseDown);
}
function onBodyMouseDown(event){
	if (!(event.target.id == "rMenu" || $(event.target).parents("#rMenu").length>0)) {
		rMenu.css({"visibility" : "hidden"});
	}
}
var addCount = 1;
function shp_onOpen() {
	var dest_path = "";
	var nodes = zTree.getSelectedNodes();
    var node = nodes[0];
	var parent = null;

	while(node!=null){
		dest_path="/"+node.name+dest_path;
		parent = node.getParentNode();
		node = parent;
	}
    
	node = nodes[0];
	var cmd_str = "{\"PID\":1001,\"DEV_ID\":";
	cmd_str+=node.DEV_ID;
	cmd_str+=",\"PATH\":\"";
	cmd_str+=dest_path+"\",\"OP_TYPE\":\"OPEN\"}";
	$.post(
		"/action/SmartHome",
		cmd_str,
		function(data,status){
			//alert("Data: " + data + "\nStatus: " + status);
		}
	);	
	hideRMenu();
}
function shp_onClose() {
	var dest_path = "";
	var nodes = zTree.getSelectedNodes();
    var node = nodes[0];
	var parent = null;

	while(node!=null){
		dest_path="/"+node.name+dest_path;
		parent = node.getParentNode();
		node = parent;
	}
    
	node = nodes[0];
	var cmd_str = "{\"PID\":1001,\"DEV_ID\":";
	cmd_str+=node.DEV_ID;
	cmd_str+=",\"PATH\":\"";
	cmd_str+=dest_path+"\",\"OP_TYPE\":\"CLOSE\"}";
	$.post(
		"/action/SmartHome",
		cmd_str,
		function(data,status){
			alert("Data: " + data + "\nStatus: " + status);
		}
	);	
	hideRMenu();
}
function shp_onHelp() 
{
	hideRMenu();
}
function resetTree() {
	hideRMenu();
	$.fn.zTree.init($("#treeDemo"), setting, zNodes);
}
