#!/bin/sh
exec ${GUILE-guile} -s $0 "$@" # -*-scheme-*-
!#

;; Simple Image Browser
;;
;; Created:    <2001-06-17 18:08:20 foof>
;; Time-stamp: <2003-11-20 00:21:05 ttn>
;; Author:     Alex Shinn <foof@debian.org>


;; load the SDL module and some useful srfi's
(use-modules ((sdl sdl) #:renamer (symbol-prefix-proc 'SDL:))
             (srfi srfi-1)
             (srfi srfi-2))

;; initialize the video subsystem
(SDL:init '(SDL_INIT_VIDEO))

;; directory to search for images in
(define image-dir "/usr/share/pixmaps/")

;; utility to test if a path is a directory
(define (file? f)
  (let* ((stats (stat f))
         (type (stat:type stats)))
    (eq? type 'regular)))

;; build a ring of image file names
(define image-ring
  (let ((dir (opendir image-dir)))
    (letrec ((D (lambda (ls)
                  (let ((file (readdir dir)))
                    (if (eof-object? file)
                        (begin (closedir dir) ls)
                        (D (cons (string-append image-dir file)
                                 ls)))))))
      (apply circular-list (reverse (filter file? (D '())))))))

;; functions to cycle through the ring
(define (next-image)
  (let ((next (car image-ring)))
    (set! image-ring (cdr image-ring))
    next))

(define (prev-image)
  (let ((orig image-ring))
    (while (not (eq? (cddr image-ring) orig))
           (set! image-ring (cdr image-ring)))
    (let ((image (car image-ring)))
      (set! image-ring (cdr image-ring))
      image)))

;; display an image given a filename
(define (show file)
  (and-let* ((image (SDL:load-image file)))
    (SDL:set-video-mode (SDL:surface:w image) (SDL:surface:h image) 24)
    (SDL:blit-surface image)
    (SDL:flip)))

;; show the first image
(show (next-image))

;; event handler
(let handle ((e (SDL:make-event)))
  (if (SDL:wait-event e)
      (case (SDL:event:type e)
        ((SDL_KEYDOWN)
         (case (SDL:event:key:keysym:sym e)
           ((SDLK_LEFT SDLK_BACKSPACE)
            (show (prev-image)))
           ((SDLK_RIGHT SDLK_SPACE)
            (show (next-image)))
           ((SDLK_ESCAPE SDLK_q)
            (SDL:quit)
            (quit))))))
  (handle e))

;;; imagebrowser.scm ends here
