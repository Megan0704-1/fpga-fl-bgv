import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader
from torchvision import datasets, transforms

class TinyNet(nn.Module):
    def __init__(self):
        super().__init__()
        self.layers = nn.Sequential(
                nn.Conv2d(1, 8, 3),
                nn.ReLU(),
                nn.MaxPool2d(2),
                nn.Flatten(),
                nn.Linear(8*13*13, 10)
                )
    def forward(self, x):
        return self.layers(x)

device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
model = TinyNet().to(device)

transform = transforms.Compose([
    transforms.ToTensor(),
    transforms.Normalize((0.1307,), (0.3081,))
    ])

train_dataset = datasets.MNIST(root="./data", train=True, download=True, transform = transform)
test_dataset = datasets.MNIST(root="./data", train=False, download=True, transform = transform)

train_loader = DataLoader(train_dataset, batch_size = 64, shuffle=True)
test_loader = DataLoader(test_dataset, batch_size=1000)

criterion = nn.CrossEntropyLoss()
optimizer = optim.Adam(model.parameters(), lr=0.001)

def train(epochs=5):
    model.train()
    for epoch in range(epochs):
        total_loss = 0
        correct = 0
        for batch_idx, (data, target) in enumerate(train_loader):
            data, target = data.to(device), target.to(device)

            optimizer.zero_grad()
            output = model(data)
            loss = criterion(output, target)
            loss.backward()
            optimizer.step()

            total_loss += loss.item()
            pred = output.argmax(dim=1)
            correct += (pred == target).sum().item()

        # Print epoch stats
        avg_loss = total_loss / len(train_loader)
        acc = 100. * correct / len(train_loader.dataset)
        print(f'Epoch {epoch+1}: Loss={avg_loss:.4f}, Accuracy={acc:.2f}%')

def evaluate():
    model.eval()
    test_loss = 0
    correct = 0
    with torch.no_grad():
        for data, target in test_loader:
            data, target = data.to(device), target.to(device)
            output = model(data)
            test_loss += criterion(output, target).item()
            pred = output.argmax(dim=1)
            correct += (pred == target).sum().item()
    
    test_loss /= len(test_loader)
    acc = 100. * correct / len(test_loader.dataset)
    print(f'\nTest set: Loss={test_loss:.4f}, Accuracy={acc:.2f}%')

if __name__ == "__main__":
    train(epochs=5)
    evaluate()
    torch.save(model.state_dict(), "tinyNet.pth")
