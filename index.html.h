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
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/css/bootstrap.min.css" integrity="sha384-TX8t27EcRE3e/ihU7zmQxVncDAy5uIKz4rEkgIXeMed4M0jlfIDPvg6uqKI2xXr2" crossorigin="anonymous">
  </head>
  <body>
    <h2>Select City</h2>
    <form action="/set" method="GET" class="form-inline" style="padding: 1%">
          <div class="form-group mb-2">
             <select name="city" id = "city" class="form-control mb-2">
               <option value = "Kyiv">Kyiv</option>
               <option value = "Brusyliv">Brusyliv</option>
               <option value = "Moscow">Moscow</option>
               <option value = "Sydney">Sydney</option>
             </select>
           </div>
           <div class="form-group mb-2">
             <button type="submit" class="btn btn-primary mb-2" style="margin-left: 15%">OK</button>
          </div>
    </form>
  </body>
</html>
    )===";
}
