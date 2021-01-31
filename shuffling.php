<html>
<head>
<title>DNA Shuffling Online</title>
<meta name="viewport" content="width=device-width, initial-scale=1.0">

<script type="text/javascript">
window.onload = function() {



var result_sequence = document.getElementsByClassName('result_sequence')[0];
var result_graph = document.getElementsByClassName('result_graph')[0];
var button = document.getElementsByClassName('button')[0];
let text_1 = document.getElementsByClassName('sequence_1')[0].value;
let text_2 = document.getElementsByClassName('sequence_2')[0].value;


function createRequestObject()   
    {  
        try { return new XMLHttpRequest() }  
        catch(e)   
        {  
            try { return new ActiveXObject('Msxml2.XMLHTTP') }  
            catch(e)   
            {  
                try { return new ActiveXObject('Microsoft.XMLHTTP') }  
                catch(e) { return null; }  
            }  
        }  

    }  

button.onclick = async function (){


let showSequence = new Promise(function(resolve, reject) {

var xhr = createRequestObject();  

if(xhr)   
        {  
            
            xhr.open('get', 'alignment.php?function=SequenceAligner&sequence_1=' + text_1 + '&sequence_2=' + text_2);  

            xhr.onreadystatechange = function ()   
            {  
                if(xhr.readyState == 4)   
                {  

                   result_sequence.innerHTML = xhr.responseText;
		   result_sequence.style.display = "inline";
		   resolve(xhr.responseText);

                }  
            }  
            xhr.send();      
        }  

});

let useless = await showSequence;


let showGraph = new Promise(function(resolve, reject) {

var xhr = createRequestObject();  

if(xhr)   
        {  
            
            xhr.open('get', 'alignment.php?function=DrawGraph&sequence_1=' + text_1 + '&sequence_2=' + text_2);  

            xhr.onreadystatechange = function ()   
            {  
                if(xhr.readyState == 4)   
                {  
		   result_graph.style.display = "inline";
		   let array = JSON.parse(xhr.responseText);
		   let counter = 0;
		   let max;
		   for (let key in array){

		   counter++;

		   if (counter == 1) max = array[key];
		   else {
		   if (array[key]>max) max = array[key];
		   }

		   }
		   let canvas_height = 0.9 * result_graph.clientHeight;
		   let canvas_width = 0.8 * result_graph.clientWidth;
		   let width_unit = canvas_width / counter;
		   let height_unit = canvas_height / max;
		   var ctx = result_graph.getContext('2d');
		   ctx.clearRect(0, 0, result_graph.clientWidth, result_graph.clientHeight);
		   let i = 0;
		   for (let key in array){
		   ctx.fillRect((result_graph.clientWidth-canvas_width)/2+width_unit*i,result_graph.clientHeight-(result_graph.clientHeight-canvas_height)/2-height_unit*array[key],width_unit,height_unit*array[key]);
		   i++;
		   }




		   
		   resolve(xhr.responseText);

                }  
            }  
            xhr.send();      
        }  

});

useless = await showGraph;



}








}
</script>


<style type="text/css">

Remove default margins, fix height html, body { height: 100%; margin: 0; padding: 0; }

@media screen and (min-device-width: 1441px) and (max-device-width: 1920px) {

.all_in_all {
width: 100%;
display: flex;
justify-content: center;
align-items: center;
flex-direction: column;
background-color: #FFFFFF;
font-family: Arial;
}

.sequence_1 {
width: 500px;
height: 100px;
margin: 20px;
font-family: Times New Roman;
border: 0px;
resize: none;
}

.sequence_2 {
width: 500px;
height: 100px;
margin: 20px;
font-family: Times New Roman;
border: 0px;
resize: none;
}

.row {
display: flex;
justify-content: center;
align-items: center;
flex-direction: row;
flex-wrap: wrap;
box-shadow: 0 30px 90px rgba(0,0,0,0.2);
background-color: #FFFFFF;
margin-top: 10vh;
}

.button {
-moz-border-radius: 7px;
-webkit-border-radius: 7px;
-khtml-border-radius:7px;
border-radius: 7px;

padding: 5px;
margin: 20px;
background-color: #7dff93;
color: #FFFFFF;
cursor: pointer;
}


.v_border{
height: 90px;
width: 0px;
border: 1px solid;
border-color: #a1a1a1;
-moz-border-radius: 100px;
-webkit-border-radius: 100px;
-khtml-border-radius:100px;
border-radius: 100px;
}

.result_sequence {
width: 1000px;
box-shadow: 0 30px 90px rgba(0,0,0,0.2);
letter-spacing: 2px;
font-family: monospace;
word-break:break-all;
display: none;
}

.result_graph {
box-shadow: 0 30px 90px rgba(0,0,0,0.2);
letter-spacing: 2px;
font-family: monospace;
word-break:break-all;
margin-top: 40px;
display: none;	
}


}



@media screen and (min-device-width: 1921px) {  /* //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */




}





@media screen and (min-device-width: 1024px) and (max-device-width: 1440px), screen and (max-device-height: 905px) and (max-device-width: 1920px) and (min-device-width: 1024px) { /* ////////////////////////////////////////////////// */




}



@media screen and (min-device-width: 769px) and (max-device-width: 1024px) { /* ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */




}




@media screen and (min-device-width: 525px) and (max-device-width: 768px), screen and (min-device-width: 320px) and (max-device-width: 425px) and (orientation: landscape) { /* ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */



}


@media screen and (min-device-width: 425px) and (max-device-width: 524px) { /* ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */



}




@media screen and (min-device-width: 320px) and (max-device-width: 425px) and (orientation: portrait) { /* ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */





}






</style>



</head>
<body>
<div class="all_in_all">


<div class="row">
<textarea class="sequence_1" placeholder="Sequence 1"></textarea>
<div class="v_border"></div>
<textarea class="sequence_2" placeholder="Sequence 2"></textarea>
</div>

<div class="button">Calculate</div>

<div class="result_sequence"></div>
<canvas class="result_graph" width="1000" height="400"></canvas>




































</div>
</body>
</html>