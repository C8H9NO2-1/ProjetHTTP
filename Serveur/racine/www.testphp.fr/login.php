<?php 
// login.php
session_start();

// Si l'utilisateur est connecté, on le redirige vers index.php
if (isset($_SESSION['identity'])) {
    header('Location: index.html');
    exit;
}

// On vérifie que le formulaire est soumis.
$submitted = false;
if ($_SERVER['REQUEST_METHOD']=='POST') {
    
    $submitted = true;
    
    // On récupère les données du formulaire
    $email = $_POST['email'];
    $password = $_POST['password'];
    
    // On authentifie l'utilisateur
    $authenticated = false;
    if ($email=='admin@example.com' && $password=='Secur1ty') {
        $authenticated = true;
        
        // On enregistre son identité en session.
        $_SESSION['identity'] = $email;
        
        // On redirige l'utilisateur vers la page index.php.
        header('Location: index.html');
        exit;
    }
}
?>

<!DOCTYPE html>
<html>
    <head>
        <title>Page de connexion</title>
    </head>
    <body>
        <h1>Connexion</h1>
        <?php if ($submitted && !$authenticated): ?>
            <div class="alert">
                Les informations d'identification sont invalides.
            </div>
        <?php endif; ?>
        <form name="login-form" action="/login.php" method="POST">
            <label for="email">E-mail</label>
            <input type="text" name="email">
            <br>
            <label for="password">Mot de passe</label>
            <input type="password" name="password">
            <br>
            <input type="submit" name="submit" value="Connexion">
        </form>
    </body>
</html>
