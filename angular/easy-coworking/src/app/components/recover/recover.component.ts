import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { ReactiveFormsModule, FormBuilder, Validators, FormGroup } from '@angular/forms';
import { Router, RouterModule } from '@angular/router';
import { HttpClientModule } from '@angular/common/http';
import { AuthService } from '../../services/auth/auth.service';
import { TOAST_TYPE } from '../../shared/components/toast/toast.component';
import { ToastService } from '../../services/toast/toast.service';

@Component({
  selector: 'app-recover',
  standalone: true,
  providers: [AuthService],
  imports: [CommonModule, ReactiveFormsModule, RouterModule, HttpClientModule],
  templateUrl: './recover.component.html',
  styleUrl: './recover.component.css'
})
export class RecoverComponent implements OnInit {
  recoverForm: FormGroup;

  constructor(private fb: FormBuilder, private authService: AuthService, private router: Router, private toastService: ToastService) {
    this.recoverForm = this.fb.group({
      email: ['', [Validators.required, Validators.email]]
    });
  }

  ngOnInit(): void {
  }

  get email() { return this.recoverForm.get("email"); }

  submit() {
    const getData = { ...this.recoverForm.value };
    const email = getData.email;

    this.authService.requestResetPassword(email).subscribe({
      next: (data: any) => {
        sessionStorage.clear();
        this.router.navigate(['/login']);
        this.toastService.showToast('Se ha enviado un correo', 'con las instrucciones para recuperar su contraseña', TOAST_TYPE.success);
      },
      error: (error: any) => {
        this.toastService.showToast('Error', 'Ha ocurrido un error: ' + error, TOAST_TYPE.error);
      }
    });
  }

}
