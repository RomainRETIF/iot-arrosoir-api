const express = require('express');

const cors = require('cors');
const bodyParser = require('body-parser');
const app = express();
app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json())
app.use(cors());
app.options('*', cors());
app.use(express.json());

//Import routes
const parametresRoutes = require('./routes/parametres');
app.use('/api/parametres', parametresRoutes);

const historiqueRoutes = require('./routes/historique');
app.use('/api/historique', historiqueRoutes);

app.get('/', (req, res) => {
    res.send("Welcome to the bakery.");
});

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => `Listening on http://localhost:${PORT} ...`);