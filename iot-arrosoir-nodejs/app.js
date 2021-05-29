const express = require('express');

const app = express();
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