const express = require('express');
const fs = require('fs');
const format = require('../joi_request_format');
const connection = require("../connection.js");
const { query } = require('../connection.js');

const router = express.Router();

router.get('/', (req, res) =>{
    connection.query("SELECT idhistorique, date_heure, temperature, humidite, luminosite, duree_arrosage FROM historique ORDER BY date_heure DESC", (err, result, fields) => {
        if(err) throw err;
        return res.status(200).send(result);
    });
});

router.post('/add', (req, res) => {
    console.log({req})
    console.log(req.body);
    const bodyCorrect = format.putBodyFormat.validate(req.body);
    if(bodyCorrect.error)
        return res.status(400).send(bodyCorrect.error.details[0].message);
    let query = "INSERT INTO historique(temperature, humidite, luminosite, duree_arrosage, date_heure) VALUES (" + req.body.temperature + ", " + req.body.humidite + ", " + req.body.luminosite + ", " + req.body.duree_arrosage + ", '" + new Date().toISOString().slice(0, 19).replace('T', ' ') + "')";
    connection.query(query, (err, result, fields) => {
        if(err) throw err;
        return res.status(200).send(result);
    });
});

module.exports = router;