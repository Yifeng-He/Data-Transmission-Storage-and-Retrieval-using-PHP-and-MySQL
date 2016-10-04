<html>
<head>
<title>User Information</title>
</head>
<body>
<div class="cssstyle">

<h2> User Info Table </h2>

<!--Start of PHP Script-->
<?php
//Step 1: connect to a MySQL database
$db = mysql_connect("localhost","yhe","222222");
if (!$db) {
	die("Database connection failed miserably: " . mysql_error());
}

//Step 2: select a database
$db_select = mysql_select_db("augmented_reality",$db);
if (!$db_select) {
	die("Database selection also failed miserably: " . mysql_error());
}

//Step 3: get the query result
$result = mysql_query("SELECT * FROM userInfo", $db);
if (!$result) {
	die("Database query failed: " . mysql_error());
}

//Step 4: disconnect MySQL
mysql_close($db);

//Step 5: show the data on HTML table
echo "<table>";

# table header
echo "<tbody align='left'>";
echo "<tr><th>User ID</th><th>User Name</th><th>Phone Number</th><th>Email</th><th>Age</th><th>Latitude</th><th>Longitude</th><th>Start Time</th></tr>";
echo "</tbody>";

# table content
echo "<tbody align='left'>";

while ($row = mysql_fetch_array($result)) {
	echo "<tr><td style='width: 70px;'>".$row[0]."</td><td style='width: 150px;'>".$row[1]."</td><td style='width: 150px;'>".$row[2]."</td><td style='width: 200px;'>".$row[3]."</td><td style='width: 50px;'>".$row[4]."</td><td style='width: 100px;'>".$row[5]."</td><td style='width: 100px;'>".$row[6]."</td><td style='width: 150px;'>".$row[7]."</td></tr>";
}

?>
<!--End of PHP Script-->

</tbody>
</table>

</div>
</body>
</html>
 



