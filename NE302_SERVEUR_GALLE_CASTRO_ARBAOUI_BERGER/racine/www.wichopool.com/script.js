let firstImage = document.getElementById("firstImage");
let secondImage = document.getElementById("secondImage");
let thirdImage = document.getElementById("thirdImage");

let firstDescription = document.getElementById("firstDescription");
let secondDescription = document.getElementById("secondDescription");

let count1 = 0;
let count2 = 0;
let count3 = 0;

// On change le texte lorsque l'image est cliquée
firstImage.addEventListener("click", function () {
    count1++;
    if (count1 % 2 == 0) {
        firstDescription.innerHTML = "It seems like something is missing in the grand scheme of things.";
    } else {
        firstDescription.innerHTML = "It seems like something is mis/<br><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;/sing in the grand scheme of things.";
    }
});

secondImage.addEventListener("click", function () {
    count2++;
    if (count2 % 2 == 0) {
        secondDescription.innerHTML = "That was actually a lot more fun than what you think.";
    } else {
        secondDescription.innerHTML = "Just kidding, it was a total nigthmare.";
    }
});

thirdImage.addEventListener("click", function () {
    count3++;
    switch (count3 % 4) {
        case 0:
            thirdImage.src = "img/dark-matter.jpg";
            break;
        case 1:
            thirdImage.src = "img/dark-matter2.jpeg";
            break;
        case 2:
            thirdImage.src = "img/dark-matter3.jpeg";
            break;
        case 3:
            thirdImage.src = "img/dark-matter4.jpeg";
            break;
    }
});
