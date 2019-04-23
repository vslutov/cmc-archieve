import numpy as np
import matplotlib.pyplot as plt
import skimage.io as skio

def display_image_in_actual_size(img, dpi = 80):
    height = img.shape[0]
    width = img.shape[1]

    # What size does the figure need to be in inches to fit the image?
    figsize = width / float(dpi), height / float(dpi)

    # Create a figure of the right size with one axes that takes up the full figure
    fig = plt.figure(figsize=figsize, dpi=dpi)
    ax = fig.add_axes([0, 0, 1, 1])

    # Hide spines, ticks, etc.
    ax.axis('off')

    # Display the image.
    ax.imshow(img, cmap='gray')

#     plt.show()
    
    return fig, ax

def save_mask(fname, snake, img):
    plt.ioff()
    fig, ax = display_image_in_actual_size(img)
    ax.fill(snake[:, 0], snake[:, 1], '-b', lw=3)
    ax.axes.get_xaxis().set_visible(False)
    ax.axes.get_yaxis().set_visible(False)
    ax.set_frame_on(False)
    fig.savefig(fname, pad_inches=0, bbox_inches='tight', dpi='figure')
    plt.close(fig)
    
    mask = skio.imread(fname)
    blue = ((mask[:,:,2] == 255) & (mask[:,:,1] < 255) & (mask[:,:,0] < 255)) * 255
    skio.imsave(fname, blue)
    plt.ion()
    
def display_snake(img, init_snake, result_snake):
    fig, ax = display_image_in_actual_size(img)
    ax.plot(init_snake[:, 0], init_snake[:, 1], '-r', lw=2)
    ax.plot(result_snake[:, 0], result_snake[:, 1], '-b', lw=2)
    ax.set_xticks([]), ax.set_yticks([])
    ax.axis([0, img.shape[1], img.shape[0], 0])
    plt.show()