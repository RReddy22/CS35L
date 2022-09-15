(defun gps-line ()
  "Print the current buffer line number and narrowed line number of point."
  (interactive)
  (let ((start (point-min)) ; point-min represents the point that we are currently on
        (n (line-number-at-pos)))
    (if (= start 1)
        (message "Line %d/%d" n(count-matches "\n" (point-min) (point-max))) ;count-matches is one of the better choices for an efficient expression and point-max represents the number of points in the file total
      (save-excursion
        (save-restriction
          (widen)
          (message "line %d (narrowed line %d)"
                   (+ n (line-number-at-pos start) -1) n))))))                                                                                                                                                                                                                                                                                                                                                                                                                                                     
