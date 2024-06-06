<?php 
// index.php
session_start();

// Si l'utilisateur est connecté, on récupère son identité via la session.
$identity = null;
if (isset($_SESSION['identity'])) {
    $identity = $_SESSION['identity'];
}
?>

<!DOCTYPE html>
<html>
    <head>
        <title>Page d'accueil</title>
    </head>
    <body>
        <h1>Page d'accueil</h1>
        <?php if ($identity==null): ?>
        <a href="login.php">Connexion</a>
        <?php else: ?>
        <strong>Bienvenue, <?= $identity ?></strong> <a href="index.html">Site web</a> <a href="logout.php">Déconnexion</a>
        <?php endif; ?>
        
        <p>
            Ceci est un test d'un site PHP pour un projet
        </p>
    </body>
</html>
