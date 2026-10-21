// This page (index.html) is a static landing page — it doesn't need direct Supabase calls itself.
// The shared client is still initialized via supabase-config.js (loaded in index.html) so it's ready
// if you add analytics or a waitlist form here later; nothing to do in this file for that.

document.addEventListener('DOMContentLoaded', () => {
  
  // Navbar Scroll Effect
  const navbar = document.getElementById('navbar');
  window.addEventListener('scroll', () => {
    if (window.scrollY > 50) {
      navbar.classList.add('scrolled');
    } else {
      navbar.classList.add('scrolled'); // keep it always a bit styled if you want, or toggle
      if(window.scrollY <= 50) navbar.classList.remove('scrolled');
    }
  });

  // Smooth Scrolling for Nav Links
  document.querySelectorAll('a[href^="#"]').forEach(anchor => {
    anchor.addEventListener('click', function (e) {
      e.preventDefault();
      const target = document.querySelector(this.getAttribute('href'));
      if(target) {
        target.scrollIntoView({
          behavior: 'smooth'
        });
      }
    });
  });

  // FAQ Accordion
  const accordionItems = document.querySelectorAll('.accordion-item');
  accordionItems.forEach(item => {
    const header = item.querySelector('.accordion-header');
    header.addEventListener('click', () => {
      // Close other items
      accordionItems.forEach(otherItem => {
        if (otherItem !== item) {
          otherItem.classList.remove('open');
        }
      });
      // Toggle current item
      item.classList.toggle('open');
    });
  });

  // Contact Form Submission (Mock)
  const contactForm = document.getElementById('contactForm');
  if (contactForm) {
    contactForm.addEventListener('submit', (e) => {
      e.preventDefault();
      const btn = contactForm.querySelector('button[type="submit"]');
      const originalText = btn.innerText;
      btn.innerText = 'Sending...';
      btn.disabled = true;
      
      // Simulate API call
      setTimeout(() => {
        btn.innerText = 'Message Sent!';
        btn.style.background = 'var(--success)';
        contactForm.reset();
        
        setTimeout(() => {
          btn.innerText = originalText;
          btn.disabled = false;
          btn.style.background = '';
        }, 3000);
      }, 1500);
    });
  }
  
  // Simple intersection observer for scroll animations (fade in up)
  const observerOptions = {
    root: null,
    rootMargin: '0px',
    threshold: 0.1
  };
  
  const observer = new IntersectionObserver((entries, observer) => {
    entries.forEach(entry => {
      if (entry.isIntersecting) {
        entry.target.style.opacity = '1';
        entry.target.style.transform = 'translateY(0)';
        observer.unobserve(entry.target);
      }
    });
  }, observerOptions);
  
  // Apply initial styles and observe
  const animatedElements = document.querySelectorAll('.glass-card, .hex-card, .feat-card, .timeline-item, .section-header');
  animatedElements.forEach(el => {
    el.style.opacity = '0';
    el.style.transform = 'translateY(30px)';
    el.style.transition = 'opacity 0.6s ease-out, transform 0.6s ease-out';
    observer.observe(el);
  });
});
