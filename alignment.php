<?php


$blosum = array ( "A" => array (      "A" => 4,
					"R" => -1,
					"N" => -2,
					"D" => -2,
					"C" => 0,
					"Q" => -1,
					"E" => -1,
					"G" => 0,
					"H" => -2,
					"I" => -1,
					"L" => -1,
					"K" => -1,
					"M" => -1,
					"F" => -2,
					"P" => -1,
					"S" => 1,
					"T" => 0,
					"W" => -3,
					"Y" => -2,
					"W" => 0,
				),
);




$aligned_given_line = "";
$aligned_reference_line = "";





if ($_GET['function'] == "SequenceAligner") SequenceAligner();
if ($_GET['function'] == "DrawGraph") DrawGraph();

function SequenceAligner ($line_1 = "", $line_2 = "", $print = true, $for_graph = false){

global $blosum;

if ($for_graph == false) {
$sequence_line_1 = $_GET['sequence_1'];
$sequence_line_2 = $_GET['sequence_2'];
}
else {
$sequence_line_1 = $line_1;
$sequence_line_2 = $line_2;

}

$x = str_split($sequence_line_1);
$y = str_split($sequence_line_2);

$d_row = 0;
$d_column = -2;
$d = -2;
$k;
$l;

$nw = [[]];
$direction = array();

for ($i = 0; $i < count($x)+1; $i++){
    for ($j = 0; $j < count($y)+1; $j++)
        if ($i == 0) {
            $nw[$i][$j] = $d_row;
            $d_row = $d_row + $d;
            if ($j > 0) {
                $k = $j - 1;
                $direction["$i" . "$j"] = "$i" . " " . "$k";
            }
        }
        else {
            if ($j == 0) {
                $nw[$i][$j] = $d_column;
                $d_column = $d_column + $d;
                $k = $i - 1;
                $direction["$i" . "$j"] = "$k" . " " . "$j";
            }
            else{
                if ($y[$j-1] == $x[$i-1]) $score = 1;
                if ($y[$j-1] != $x[$i-1]) $score = -1;
                $value_1 = $nw[$i - 1][$j] + $d;
                $value_2 = $nw[$i][$j - 1] + $d;
                $value_3 = $nw[$i - 1][$j - 1] + $score;
                $nw[$i][$j] = max($value_1, $value_2, $value_3);
                if ($nw[$i][$j] == $value_1){
                    $k = $i - 1;
                    $direction["$i" . "$j"] = "$k" . " " . "$j";  
                }
                if ($nw[$i][$j] == $value_2) {
                    $k = $j - 1;
                    $direction["$i" . "$j"] = "$i" . " " . "$k";
                }
                if ($nw[$i][$j] == $value_3) {
                    $k = $i - 1;
                    $l = $j - 1;
                    $direction["$i" . "$j"] = "$k" . " " . "$l";
                }
            }
        }
    
    
}



$j = count($y);
$i = count($x);



global $aligned_given_line, $aligned_reference_line;

do {
    
   $value = explode(" ", $direction["$i" . "$j"]);
   $value[0] = intval($value[0]);
   $value[1] = intval($value[1]);
   if($i == $value[0] && $j > $value[1]){
       $aligned_reference_line = $aligned_reference_line . "-";
       $aligned_given_line = $aligned_given_line . $y[$j-1];
       $j--;
   }
   if($i > $value[0] && $j == $value[1]){
       $aligned_given_line = $aligned_given_line . "-";
       $aligned_reference_line = $aligned_reference_line . $x[$i-1];
       $i--;
   }
   if($i > $value[0] && $j > $value[1]){
       $aligned_given_line = $aligned_given_line . $y[$j-1];
       $aligned_reference_line = $aligned_reference_line . $x[$i-1];
       $i--;
       $j--;
   }
} while ($i > 0 || $j > 0);

if($print == true){
echo $aligned_given_line;
echo "<br>";
echo $aligned_reference_line;
}

}

/////////////////////////////////////////////////////////////


function DrawGraph (){

global $blosum, $aligned_given_line, $aligned_reference_line;


$gambel = array ();
$keys = array();



for ($k=0; $k < 500; $k++) {

$sum = 0;

$edited_line = str_shuffle($_GET['sequence_1']);
$remained_line = $_GET['sequence_2'];

SequenceAligner($edited_line, $remained_line, false, true);




for ($i=0; $i<strlen($edited_line); $i++){
if ($edited_line[$i] != "-" && $remained_line[$i] != "-") $sum = $sum + $blosum[$edited_line[$i]][$remained_line[$i]];
}



if (!array_key_exists($sum, $gambel)) {
$gambel[$sum] = 1;
$keys[] = $sum;
}
else {
$gambel[$sum] = $gambel[$sum] + 1;
}

}


$all_data = array ();

for ($i=0; $i < count($keys); $i++){
$all_data[$keys[$i]]=$gambel[$keys[$i]];
}


ksort($all_data);

foreach ($all_data as $key => $value) {
//echo $key;
//echo ":";
//for ($j=0; $j < $value; $j++) echo "#";
//echo "<br>";
}


$json_string = json_encode($all_data);
echo $json_string;

}



?>