#pragma once
#include "pagehelper.h"    
namespace index_html
{
    CreatePageName; 
    HTMLPAGE = R"===(
<!DOCTYPE HTML>
<html lang = "en">
  <head>
    <title>Weather</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0"> 
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/css/bootstrap.min.css" integrity="sha384-TX8t27EcRE3e/ihU7zmQxVncDAy5uIKz4rEkgIXeMed4M0jlfIDPvg6uqKI2xXr2" crossorigin="anonymous">
<style>
body { background: gray;}
h2 {color: white; padding: 1% 2%;}	
button {margin-left: 15%}
@media (max-width : 480px) {
    mobile_fix {
        padding:0;
    }
    body {
        padding: 20% 0% 25% 25%;
    }
}
</style>
  </head>
  <body>
    <div class="row">	 
    <div class="col-md-12">    
    <h2>Select City</h2>
    <form action="/set" method="GET" class="form-inline" style="padding: 1%">
          <div class="form-group mb-2">
             <select name="city" id = "city" class="form-control mb-2">
                <option value="Kyiv">Kyiv</option>
                <option value="Brusyliv">Brusyliv</option>
		<option value="Odessa">Odessa</option>
		<option value="Kharkiv">Kharkiv</option>
		<option value="Istanbul">Istanbul</option>
		<option value="London">London</option>
                <option value="Moscow">Moscow</option>
                <option value="Sydney">Sydney</option>
		<option value="Paris">Paris</option>
		<option value="Tbilisi">Tbilisi</option>
		<option value="Athens">Athens</option>
		<option value="Madrid">Madrid</option>
		<option value="Rome">Rome</option>
		<option value="Paris">Paris</option>
		<option value="Bucharest">Bucharest</option>
		<option value="Minsk">Minsk</option>
		<option value="Vienna">Vienna</option>
		<option value="Hamburg">Hamburg</option>
		<option value="Warsaw">Warsaw</option>
		<option value="Budapest">Budapest</option>
		<option value="Barcelona">Barcelona</option>
		<option value="Munich">Munich</option>
		<option value="Milan">Milan</option>
		<option value="Belgrade">Belgrade</option>
		<option value="Prague">Prague</option>
		<option value="Sofia">Sofia</option>
		<option value="Birmingham">Birmingham</option>
		<option value="Brussels">Brussels</option>
		<option value="Cologne">Cologne</option>
             </select>
           </div>
           <div class="form-group mb-2">
             <button type="submit" class="btn btn-primary mb-2">OK</button>
          </div>
    </form>
    </div>
    </div>
  </body>
</html>
    )===";
}
