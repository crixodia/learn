import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { animate, state, style, transition, trigger } from '@angular/animations';
import { ToastService } from '../../../services/toast/toast.service';

export const TOAST_TYPE = {
  success: 'alert-success',
  error: 'alert-error',
  info: 'alert-info',
  warning: 'alert-warning'
}

@Component({
  selector: 'app-toast',
  animations: [
    trigger('toastTrigger', [
      state('open', style({
        opacity: 1,
        //transform: 'translate(0)'
      })),
      state('close', style({
        opacity: 0,
        //transform: 'translate(100)'
      })),
      transition('open <=> close', [
        animate('300ms ease-in-out')
      ])
    ])
  ],
  standalone: true,
  imports: [CommonModule],
  templateUrl: './toast.component.html',
  styleUrl: './toast.component.css'
})
export class ToastComponent implements OnInit {
  toastTitle: string = 'Default Title';
  toastMessage: string = 'Default Message';
  toastType: string = TOAST_TYPE.success;
  toastShow: boolean = false;

  constructor(private toastService: ToastService) { }

  ngOnInit(): void {
    this.toastService.registerToastComponent(this);
  }

  dimissToast(): void {
    this.toastShow = false;
  }
}
