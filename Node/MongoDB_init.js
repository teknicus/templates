var MongoClient = require('mongodb').MongoClient;
var url = "mongodb://localhost:27017/";


var dbo = db.db("<Database Name>"); //Define DB


var myquery = { <Parameter>: <Condition> }; // Define your query

dbo.collection(place).find(myquery, {  //Query the DB
    projection: {
        _id:0,
        dpoint: 1
    }
}).toArray(function(err, result) { //Convert to array if necessary
    console.log(result);

    });
});

MongoClient.connect(url, function(err, db) {
    if (err) throw err;

    var myobj = { //declare your JSON Object
        name: dname,
        phno: dphno,
        lino: dlino,
        idp: didp,
        uname: duname,
        pass: dpass
    };

    dbo.collection("drivers").insertOne(myobj, function(err, res) { //push to DB
        if (err) throw err;
        console.log("1 document inserted");
        db.close();
    });
});

db.close(); //close DB after use

