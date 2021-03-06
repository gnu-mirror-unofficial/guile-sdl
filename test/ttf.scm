;;; ttf.scm --- simple true type font test         -*- coding: utf-8 *-

;; Copyright (C) 2003, 2004, 2007-2009, 2011-2013, 2015 Thien-Thi Nguyen
;;
;; This program is free software; you can redistribute it and/or
;; modify it under the terms of the GNU General Public License as
;; published by the Free Software Foundation; either version 3 of
;; the License, or (at your option) any later version.
;;
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program.  If not, see <http://www.gnu.org/licenses/>.

(or *have-ttf* (exit-77 "ttf disabled"))

(use-modules ((sdl sdl) #:prefix SDL:)
             ((sdl ttf) #:prefix TTF:))

;; initialize SDL video
(SDL:init 'video)

;; initialize the font lib
(or (zero? (TTF:ttf-init)) (error "could not init font lib"))

;; the text to display
(define sentence (let ((ls (map symbol->string
                                '(The quick brown fox
                                      jumped over the
                                      lazy sleeping dog
                                      !!!))))
                   (set-cdr! (last-pair ls) ls)
                   ls))

;; load a font file
(define (font-loader filename)
  (lambda (size)
    (let* ((font (TTF:load-font filename size))
           (style (TTF:font:style font))
           (etc (map (lambda (proc)
                       (cons (procedure-name proc)
                             (proc font)))
                     (list TTF:font:height
                           TTF:font:ascent
                           TTF:font:descent
                           TTF:font:line-skip))))
      (cond ((zero? (random 2))
             (TTF:font:set-style! font 'underline)
             (set! style (TTF:font:style font))))
      (info "loaded ~S ~S => ~S" filename style font)
      (for-each (lambda (pair)
                  (info "\t(~A) ~S" (car pair) (cdr pair)))
                etc)
      font)))

(define fonts (let* ((filename (datafile "FreeSerifBoldItalic.ttf"))
                     (ls (map (font-loader filename)
                              '(16 32 64 128))))
                (set-cdr! (last-pair ls) ls)
                ls))

;; initialize the video mode
(define test-rect (SDL:make-rect 0 0 640 480))
(SDL:set-video-mode (SDL:rect:w test-rect) (SDL:rect:h test-rect) 16)

(set! *random-state* (seed->random-state (current-time)))

(define (rand-rect font word)
  (call-with-values (lambda () (TTF:text-wh font word))
    (lambda (w h)
      (SDL:make-rect (random (- (SDL:rect:w test-rect) w))
                     (random (- (SDL:rect:h test-rect) h))
                     w h))))

(define rand-color
  (lambda ()
    (let ((c (SDL:make-color (random #xff) (random #xff) (random #xff))))
      (or (SDL:color? c)
          (error "make-color failed"))
      (let ((r (SDL:color:r c))
            (g (SDL:color:g c))
            (b (SDL:color:b c)))
        (info "(color) ~A ~A ~A" r g b))
      c)))

;; clear the screen
(SDL:fill-rect (SDL:get-video-surface) test-rect #xffffff)
(SDL:flip)

(define (renderer proc)
  (lambda (font string)
    (proc font string (rand-color) (case (random 3)
                                     ((0) #t)
                                     ((1) #f)
                                     ((2) (rand-color))))))

;; write the text in random locations with random colors
(let ((src-rect (SDL:make-surface (SDL:rect:w test-rect)
                                  (SDL:rect:h test-rect)))
      (screen (SDL:get-video-surface)))
  (let loop ((i 420) (words sentence) (fonts fonts))
    (or (zero? i)
        (let* ((word (car words))
               (font (car fonts))
               (text (TTF:render-text font word (rand-color)
                                      (case (random 3)
                                        ((0) #t)
                                        ((1) #f)
                                        ((2) (rand-color)))))
               (dst-rect (rand-rect font word)))
          (SDL:blit-surface text test-rect screen dst-rect)
          (SDL:update-rect screen dst-rect)
          (loop (1- i) (cdr words) (cdr fonts))))))

(define (spew-utf-8)
  (let ((infinity "?????")
        (test-w (SDL:rect:w test-rect))
        (test-h (SDL:rect:h test-rect))
        (screen (SDL:get-video-surface))
        (render (renderer TTF:render-utf8)))
    (let loop ((i 42) (fonts fonts))
      (or (zero? i)
          (let* ((font (car fonts))
                 (text (render font infinity)))
            (call-with-values (lambda () (TTF:utf8-wh font infinity))
              (lambda (w h)
                (let ((dst-rect (SDL:make-rect (random (- test-w w))
                                               (random (- test-h h))
                                               w h)))
                  (SDL:blit-surface text test-rect screen dst-rect)
                  (SDL:update-rect screen dst-rect)
                  (loop (1- i) (cdr fonts))))))))))

(define (spew-at)
  (let ((at #\@)
        (test-w (SDL:rect:w test-rect))
        (test-h (SDL:rect:h test-rect))
        (screen (SDL:get-video-surface))
        (render (renderer TTF:render-glyph)))
    (let loop ((i 42) (fonts fonts))
      (or (zero? i)
          (let* ((font (car fonts))
                 (text (render font at))
                 (w (SDL:surface:w text))
                 (h (SDL:surface:h text))
                 (dst-rect (SDL:make-rect (random (- test-w w))
                                          (random (- test-h h))
                                          w h)))
            (call-with-values (lambda () (TTF:font:glyph-xXyYa font at))
              (lambda metrics
                (info "metrics => ~S" metrics)))
            (SDL:blit-surface text test-rect screen dst-rect)
            (SDL:update-rect screen dst-rect)
            (loop (1- i) (cdr fonts)))))))

(spew-utf-8)
(spew-at)

;; clean up
(SDL:delay 1000)
(TTF:ttf-quit)
(exit (SDL:quit))

;;; ttf.scm ends here
