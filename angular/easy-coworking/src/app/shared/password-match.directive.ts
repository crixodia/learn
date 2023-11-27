import { ValidatorFn, AbstractControl, ValidationErrors } from "@angular/forms";

export const password_pattern = /^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)[a-zA-Z\d]{8,}$/;
export const name_pattern = /^[a-zA-Z]+.*$/;

export const passwordMatchValidator: ValidatorFn = (control: AbstractControl): ValidationErrors | null => {
    const contrasena = control.get('contrasena');
    const confirm_contrasena = control.get('confirm_contrasena');

    if (!contrasena || !confirm_contrasena) return null;

    return contrasena.value === confirm_contrasena.value ? null : { passwordMismatch: true };
}