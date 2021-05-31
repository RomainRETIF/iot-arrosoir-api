const express = require('express');
const fs = require('fs');
const format = require('../joi_request_format');
const connection = require("../connection.js");
const { query } = require('../connection.js');

const DATABASE = './data/products.json';

const router = express.Router();

router.get('/', (req, res) =>{
    connection.query("SELECT idparametre, temperature, humidite, luminosite, duree_arrosage FROM parametres", (err, result, fields) => {
        if(err) throw err;
        return res.status(200).send(result);
    });
});

router.put('/update', (req, res) => {
    const bodyCorrect = format.putBodyFormat.validate(req.body);
    if(bodyCorrect.error)
        return res.status(400).send(bodyCorrect.error.details[0].message);
    let query = "UPDATE parametres SET temperature = " + req.body.temperature + ", humidite = " + req.body.humidite + ", luminosite = " + req.body.luminosite + ", duree_arrosage = " + 2;
    connection.query(query, (err, result, fields) => {
        if(err) throw err;
        return res.status(200).send(result);
    });
});

module.exports = router;