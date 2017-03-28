/**
 * Created by mine on 2017/3/27.
 */

var flashvarsVideoNewControls = {
    source: "http://jaris.sourceforge.net/files/audio.mp3",
    type: "video",
    streamtype: "file",
    server: "",//Used for rtmp streams
    duration: "52",
    poster: "http://jaris.sourceforge.net/images/poster.png",
    autostart: "false",
    logo: "http://jaris.sourceforge.net/images/logo.png",
    logoposition: "top left",
    logoalpha: "30",
    logowidth: "130",
    logolink: "http://jaris.sourceforge.net",
    hardwarescaling: "false",
    darkcolor: "000000",
    brightcolor: "4c4c4c",
    controlcolor: "FFFFFF",
    hovercolor: "67A8C1",
    controltype: 1

};
var flashvarsAudio = {
    source: "http://jaris.sourceforge.net/files/audio.mp3",
    type: "audio",
    streamtype: "file",
    server: "",//Used for rtmp streams
    duration: "00:04:25",
    poster: "http://jaris.sourceforge.net/images/poster.png",
    autostart: "false",
    logo: "http://jaris.sourceforge.net/images/logo.png",
    logoposition: "top left",
    logoalpha: "30",
    logowidth: "130",
    logolink: "http://jaris.sourceforge.net",
    hardwarescaling: "false",
    darkcolor: "D3D3D3",
    brightcolor: "FFFFFF",
    controlcolor: "000000",
    hovercolor: "FF0000",
    controltype: 1
};
var params = {
    menu: "false",
    scale: "noScale",
    allowFullscreen: "true",
    allowScriptAccess: "always",
    bgcolor: "#000000",
    quality: "high",
    wmode: "opaque"
};
var attributes = {
    id:"JarisFLVPlayer"
};
function  start(){

    //alert(dg.rows[1].url);

    flashvarsVideoNewControls.source=dg.rows[0].url;
   swfobject.embedSWF("JarisFLVPlayer.swf", "vidio", "576px", "360px", "10.0.0", "expressInstall.swf", flashvarsVideoNewControls, params, attributes);
}

function show(i) {
    ////swfobject.removeSWF("vidio");
    //var right=document.getElementById("right");
    //var dd=document.getElementById("vidio");
    ////right.removeChild(div);
    //dd.parentNode.removeChild(dd);
    //alert("h");
    //flashvarsVideoNewControls.source=dg.rows[i].url;
    //swfobject.embedSWF("JarisFLVPlayer.swf", "vidio", "576px", "360px", "10.0.0", "expressInstall.swf", flashvarsVideoNewControls, params, attributes);

    $('#lmnpop-video-button').remove();
    $('JarisFLVPlayer').remove();
    $('#right').empty();
    $('#right').append("<div id='vidio'></div>");
    if(dg.rows[i].url.search(/.mp3/i)==-1){//视频文件
        flashvarsVideoNewControls.source=dg.rows[i].url;
        swfobject.embedSWF("JarisFLVPlayer.swf", "vidio", "576px", "360px", "10.0.0", "expressInstall.swf", flashvarsVideoNewControls, params, attributes);
    }else {
        flashvarsAudio.source=dg.rows[i].url;
        swfobject.embedSWF("JarisFLVPlayer.swf", "vidio", "576px", "360px", "10.0.0", "expressInstall.swf", flashvarsAudio, params, attributes);
    }


}
