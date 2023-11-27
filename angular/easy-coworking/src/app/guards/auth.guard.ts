import { CanActivateFn, Router } from '@angular/router';
import { inject } from '@angular/core';

export const authGuard: CanActivateFn = () => {
  if (sessionStorage.getItem('access_token')) {
    return true;
  } else {
    const router = inject(Router);
    return router.navigate(['login']);
  }
};

export const loggedGuard: CanActivateFn = () => {
  if (sessionStorage.getItem('access_token')) {
    const router = inject(Router);
    return router.navigate(['home']);
  } else {
    return true;
  }
};

