

const oos = '/wp-content/themes/vend/media/outOfStock.png'


// Function to create a grid item element
function createGridItem(item) {
    const gridItem = document.createElement('a');
    gridItem.href = item.url
    gridItem.classList.add('grid-item');
    gridItem.style = "grid-area: " + item.sku;

    const gridItemImage = document.createElement('div');
    gridItemImage.classList.add('grid-item-image');
   // gridItemImage.style.backgroundImage = "url(" + item.image + ")";
    const img = document.createElement('img');
    img.src = item.image;
    gridItemImage.appendChild(img)



    const name = document.createElement('h3');
    name.textContent = item.name;

    const id = document.createElement('span');
    id.textContent = item.sku;

    gridItem.appendChild(gridItemImage);
    if(item.stock < 1 ){
        const outOfStock = document.createElement('img');
        outOfStock.src = oos;
        outOfStock.classList.add('oosImg');
        outOfStock.classList.add('blur');
        gridItem.appendChild(outOfStock);
        gridItem.classList.add('oosPointer');
    }
   
    gridItem.appendChild(name);
    gridItem.appendChild(id);

    return gridItem;
}


// generate our empty grid
const gridPlaceholders = [
    {
        id: 'js-grid-container-one',
        letters: 'ABC',
        length: 5,
    },
    {
        id: 'js-grid-container-two',
        letters: 'DE',
        length: 10,
    },
    {
        id: 'js-grid-container-three',
        letters: 'F',
        length: 5,
    },
]

gridPlaceholders.forEach(gridPlaceholder => {
    const gridPlaceholderDiv = document.getElementById(gridPlaceholder.id);
    let gridCss = "";
    const gridLetters = [];
    for(let letterIndex = 0; letterIndex < gridPlaceholder.letters.length; letterIndex++){
        const letter = gridPlaceholder.letters.charAt(letterIndex);
        gridCss += '"';
        for(let index = 1; index <= gridPlaceholder.length; index++){
            const gridLabel = `${letter}${index}`;
            gridLetters.push(gridLabel);
            gridCss = gridCss + `${gridLabel} `;
            const gridPlaceholderItem = document.createElement('div');
            gridPlaceholderItem.classList.add('grid-placeholder');
            gridPlaceholderItem.style.gridArea = gridLabel;
            gridPlaceholderDiv.appendChild(gridPlaceholderItem);
        }
        gridCss += '" ';
    }
    gridPlaceholderDiv.style.gridTemplateAreas = gridCss


    // Put the items on the grid:
    vendingproducts.forEach(item => {
        if(gridLetters.includes(item.sku)){
            const gridItem = createGridItem(item);
            gridPlaceholderDiv.appendChild(gridItem);
        }
    });
})

