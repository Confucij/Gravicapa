function rotate_cube(id,upr) {
    if ((id=="floor9")&&(upr==null)){
        get_sensors_in_auditory(91);
        get_sensors_in_auditory(95);
        get_sensors_in_auditory(92); 
        get_sensors_in_auditory(93);
        
    }
    xmlHttp_auditory_flag[92]=upr;
    xmlHttp_auditory_flag[93]=upr; 
    xmlHttp_auditory_flag[91]=upr;
    xmlHttp_auditory_flag[95]=upr; 
                
    if (rotate_cube_flag==0){
        return;
    }
    rotate_cube_flag=0;
    divid=id;
    var div=document.getElementById(divid);     
    if (div.style.height==""){
        div.style.height=116+'px';
    }
    var qwe=div.style.height;
    if(parseInt(qwe)>116){
        outheightdiv();       
    }else{                                 
        rotate_cube_roretion(divid);   
    }
};
            
function rotate_cube_roretion(id){
    var cube = $('#'+id); 
    var next = parseInt($('#next').val(), 10); 
    cube.imagecube('change', {
        direction: $('#direction').val()
    }). 
    imagecube('rotate', next, function() { 
        $('#current').text($(cube.imagecube('current')).attr('title')); 
    });    
}

function endedRotate(current, next) { 
    $('#rotateStatus').text('ended ' + $(current).attr('title') + 
        ' » ' + $(next).attr('title')); 
    var par=$(next).parents('div')[0];
    if (flag==1){
        flag=0;
        divid=par.id;
        inheightdiv();
    }else{
        flag=1;
    }
}
              
function outheightdiv(){
    if (outheightdiv_flag==0){
        return;
    }
    outheightdiv_flag1=0;
    flag=0;
    var div=document.getElementById(divid);     
    if (div.style.height==""){
        div.style.height=116+'px';
    }
    var qwe=div.style.height;
              
    if(parseInt(qwe)>116){
        div.style.height=(parseInt(div.style.height)-2)+'px';
        var table=$("."+divid+'_class');
        $(table).height(div.style.height);
        setTimeout(outheightdiv,1);
        return;
    }                
    rotate_cube_roretion(divid);
    outheightdiv_flag1=1;
    rotate_cube_flag=1;
    return;
}
            
function inheightdiv(){
    if (inheightdiv_flag==0){
        return;
    }
    inheightdiv_flag1=0;
    var div=document.getElementById(divid);
                
    if (div.style.height==""){
        div.style.height=116+'px';
    }
    var qwe=div.style.height;
              
    if(parseInt(qwe)<400){
        div.style.height=(parseInt(div.style.height)+3)+'px';
        var table=$("."+divid+'_class');
        $(table).height(div.style.height);
        setTimeout(inheightdiv,1);
        return;
    }
    window.location ="#"+divid+"_pos";
    inheightdiv_flag1=1;
    rotate_cube_flag=1;
    flag=1;
    return;
}

