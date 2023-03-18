function getRandomColor() {
    const letters = "0123456789ABCDEF";
    let color = "#";
    for (let i = 0; i < 6; i++) {
        color += letters[Math.floor(Math.random() * 16)];
    }
    return color;
}

function isColorDark(color) {
    const r = parseInt(color.slice(1, 3), 16);
    const g = parseInt(color.slice(3, 5), 16);
    const b = parseInt(color.slice(5, 7), 16);
    const brightness = (r * 299 + g * 587 + b * 114) / 1000;
    return brightness < 128;
}

function createButtons() {
    const boxContainer = document.getElementById("boxContainer");
    const boxes = boxContainer.querySelectorAll("div[data-text]");

    boxes.forEach(box => {
        const bgColor = getRandomColor();
        box.style.backgroundColor = bgColor;
        box.style.borderColor = isColorDark(bgColor) ? "rgba(255, 255, 255, 0.3)" : "rgba(0, 0, 0, 0.3)";
        box.style.width = `${Math.floor(Math.random() * 100) + 75}px`;
        box.style.height = `${Math.floor(Math.random() * 100) + 25}px`;

        const buttonText = document.createElement("span");
        buttonText.setAttribute("data-text", "");
        buttonText.textContent = box.dataset.text;
        buttonText.style.color = isColorDark(bgColor) ? "#FFFFFF" : "#000000";
        box.appendChild(buttonText);

        const hoverText = document.createElement("span");
        hoverText.setAttribute("data-hover", "");
        hoverText.textContent = box.dataset.hoverText;
        box.appendChild(hoverText);

        box.addEventListener("click", () => {
            window.location.href = box.dataset.href;
        });
    });
}

createButtons();
