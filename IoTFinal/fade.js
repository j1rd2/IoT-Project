window.addEventListener('scroll', function() {
    var header = document.querySelector('.header');
    var main = document.querySelector('.main');
    var scrollPosition = window.pageYOffset;
    var mainOffsetTop = main.offsetTop;
    
    if (scrollPosition > mainOffsetTop) {
      header.style.backgroundColor = 'white';
      main.style.backgroundColor = 'blue';
    } else {
      header.style.backgroundColor = 'blue';
      main.style.backgroundColor = 'white';
    }
  });
  