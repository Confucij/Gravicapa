var xmlHttp=getXmlHttp();
var xmlHttp1=getXmlHttp();
var authxmlHttp1=getXmlHttp();
var logxmlHttp1=getXmlHttp();
var update_senor_xmlHttp=getXmlHttp();
var set_auditory_xmlHttp=getXmlHttp();
var xmlHttp_auditory=new Array();
var xmlHttp_auditory_flag=new Array();
var period_oprosa=1000;
function getXmlHttp(){
    var xmlhttp;
    try {
        xmlhttp = new ActiveXObject("Msxml2.XMLHTTP");
    } catch (e) {
        try {
            xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
        } catch (E) {
            xmlhttp = false;
        }
    }
    if (!xmlhttp && typeof XMLHttpRequest!='undefined') {
        xmlhttp = new XMLHttpRequest();
    }
    return xmlhttp;
}

var get_new_sensors_flag=0;
var enterNewSensors_flag=0;

function call_get_new_sensors(){
    // РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… URL РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…
    var url = "cgi-bin/site_lib/get_new_sensors.pl?";
    // РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…
    xmlHttp.open("GET", url, true);
    xmlHttp.onreadystatechange = enterNewSensors;
    // SРїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…
    xmlHttp.send(null);  
}

function get_new_sensors() {
    if (get_new_sensors_flag==1){
        return;
    }
    get_new_sensors_flag=1;
    call_get_new_sensors();
}
            
function enterNewSensors() {
    if (xmlHttp.readyState == 4) {
        var response = xmlHttp.responseText;		    
        document.getElementById("new_sensors_content").innerHTML=response;
        if (enterNewSensors_flag==1){
            setTimeout("call_get_new_sensors()",  10000);   
        }else{
            get_new_sensors_flag=0;
        }    
    }
}
            
function get_sensors_in_auditory(id) {
    // РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… URL РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…
    var url = "cgi-bin/site_lib/get_sensors_in_auditory.pl?id="+id.toString();
    if (xmlHttp_auditory[id]==null){
        xmlHttp_auditory[id]=getXmlHttp();
    }
    // РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…
    xmlHttp_auditory[id].open("GET", url, true);
    // РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…, РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…
    xmlHttp_auditory[id].onreadystatechange = function ()  {
        if (xmlHttp_auditory[id].readyState == 4) {
            var response = xmlHttp_auditory[id].responseText;
            var col = response.substr(0,1); //
            var aud = response.substr(1,col); //
            var htmlsrt=response.substr(parseInt(col)+1,response.length-col);
            document.getElementById(aud).innerHTML=htmlsrt;
            if (xmlHttp_auditory_flag[aud]==null){
                setTimeout("get_sensors_in_auditory("+aud+")",  period_oprosa);                    
            }
        };
    }
    // xmlHttp1�їС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…
    xmlHttp_auditory[id].send(null);
  
}
    
    
            
function authorization_start() {
    // РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… URL РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…
    var url = "cgi-bin/site_lib/authorization.pl?login="+document.getElementById("login").value+"&pass="+document.getElementById("pass").value;
    // РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…
    authxmlHttp1.open("GET", url, true);
    // РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…, РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…
    authxmlHttp1.onreadystatechange = result_authorization; 
    // xmlHttp1�їС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…
    authxmlHttp1.send(null);
}
            
function result_authorization() {
    if (authxmlHttp1.readyState == 4) {
        var response = authxmlHttp1.responseText;
        if (response.toString()=="ok"){            
            document.getElementById("loginbutton").style.display="none";  
            document.getElementById("errorloginmes").style.display="none";      
            authorization_form_close();       
            document.getElementById("logoutbutton").style.display="block";
        }else{
            document.getElementById("errorloginmes").style.display="block";
        } 
        checkuser();
    }else{
        checkuser();
    }
}
      
      
function  logOut_please() {
    // РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… URL РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…
    var url = "cgi-bin/site_lib/log_out.pl";               
    // РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…
    authxmlHttp1.open("GET", url, true);
    // РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…, РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…
    authxmlHttp1.onreadystatechange = logout_authorization; 
    // xmlHttp1�їС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…
    authxmlHttp1.send(null);
}
            
function logout_authorization() {
    if (authxmlHttp1.readyState == 4) {
        var response = authxmlHttp1.responseText;
        document.getElementById("errorloginmes").style.display="none";        
        authorization_form_close();                
    }    
    checkuser();
}
        



function single_checkuser(){
    // РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… URL РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…
    var url = "cgi-bin/site_lib/get_current_user.pl";               
    // РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…
    logxmlHttp1.open("GET", url, true);
    // РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…, РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…
    logxmlHttp1.onreadystatechange = cur_user_func; 
    // xmlHttp1�їС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…
    logxmlHttp1.send(null);
}

function  checkuser() {
    single_checkuser();
}

function cur_user_func() {
    if (logxmlHttp1.readyState == 4) {
        var response = logxmlHttp1.responseText;
        if (response=="0"){
            document.getElementById("curusername").textContent="Добро пожаловать";
            document.getElementById("loginbutton").style.display="block";  
            document.getElementById("logoutbutton").style.display="none";            
            document.getElementById("newsensorsbut").style.display="none";   
            document.getElementById("sensor_settings_menu").style.display="none"; 
            document.getElementById("additional_opportunities").style.display="none";
            
            $("#new_sensors").dialog("close");
            enterNewSensors_flag=0;
            return;
        }else{
            document.getElementById("curusername").textContent=response;
            document.getElementById("loginbutton").style.display="none";  
            document.getElementById("logoutbutton").style.display="block";
            document.getElementById("newsensorsbut").style.display="block";             
            document.getElementById("additional_opportunities").style.display="block";            
            document.getElementById("sensor_settings_menu").style.display="block";
            
            authorization_form_close();
            new_sensors_open();
            return;
        }        
        flag_checkuser=0; 
    }   
}   

var update_sensors_flag=1;

function update_sensors(){
    if(update_sensors_flag==0){
        return;
    }
    update_sensors_flag=0;
    // РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… URL РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…
    var url = "cgi-bin/site_lib/tcpclienttread.pl";
    // РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…
    update_senor_xmlHttp.open("POST", url, true);
    update_senor_xmlHttp.onreadystatechange = update_sensors_response_func();
    // SРїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…
    update_senor_xmlHttp.send(null); 
    setTimeout("update_sensors_response_func()",  500000);   
}

function update_sensors_response_func(){
    if (update_senor_xmlHttp.readyState == 4) {  
        update_sensors_flag=1;
        get_new_sensors();
    } 
}


function set_auditory(){
    // РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… URL РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…
    var url = "cgi-bin/site_lib/set_auditory_pl.pl?auditory="+document.getElementById("setsendial_input").value+"&id="+document.getElementById("setsendial_id").value;               
    // РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…
    set_auditory_xmlHttp.open("GET", url, true);
    // РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…, РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…
    set_auditory_xmlHttp.onreadystatechange = cur_user_func; 
    // xmlHttp1�їС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р… РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…РїС—Р…
    set_auditory_xmlHttp.send(null);
}

function set_auditory_result(){
    if (set_auditory_xmlHttp.readyState == 4) {
        var response = set_auditory_xmlHttp.responseText;
        get_new_sensors();
    }
}