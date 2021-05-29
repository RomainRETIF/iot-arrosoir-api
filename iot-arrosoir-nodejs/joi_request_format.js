const joi = require('joi');

const postBodyFormat = joi.object({
    humidite : joi.number().min(0).max(100),
    luminosite : joi.number(),
    temperature : joi.number().min(-20).max(50),
    duree_arrosage : joi.number().min(0).max(60),
});

const putBodyFormat = joi.object({
    humidite : joi.number().min(0).max(100),
    luminosite : joi.number(),
    temperature : joi.number().min(-20).max(50),
    duree_arrosage : joi.number().min(0).max(60),
});


module.exports = {postBodyFormat: postBodyFormat, putBodyFormat: putBodyFormat};